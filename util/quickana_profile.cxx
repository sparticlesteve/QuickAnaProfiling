// System includes
#include <memory>
#include <chrono>

// ROOT includes
#include "TFile.h"
#include "TError.h"

// Infrastructure includes
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "AsgTools/StatusCode.h"
#include "PATInterfaces/CorrectionCode.h"
#include "PATInterfaces/SystematicsUtil.h"

// QuickAna includes
#include "QuickAna/QuickAna.h"

// Boost
#include "boost/program_options.hpp"

// Callgrind control
#include "valgrind/callgrind.h"

namespace po = boost::program_options;

//-----------------------------------------------------------------------------
// Error checking macro
//-----------------------------------------------------------------------------
#define CHECK( ARG )                                 \
  do {                                               \
    const bool result = ARG;                         \
    if(!result) {                                    \
      ::Error(appName, "Failed to execute: \"%s\"",  \
              #ARG );                                \
      return EXIT_FAILURE;                           \
    }                                                \
  } while( false )

//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  // Application name
  const char* appName = argv[0];

  // Command line processing
  po::options_description desc(std::string(appName) + " options");
  desc.add_options()
    ("help,h", "Display this help message")
    ("input-file,i", po::value<std::string>()->required(), "Input xAOD file")
    //("output-file,o", po::value<std::string>()->required(), "Output ROOT file")
    ("num-events,n", po::value<Long64_t>()->default_value(-1),
     "Number of events to process")
    ("sys", "Activate systematics");
  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).
              options(desc).run(), vm);
    if(vm.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }
    po::notify(vm);
  }
  catch (po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl
              << desc << std::endl;
    return EXIT_FAILURE;
  }
  const std::string inputFileName = vm["input-file"].as<std::string>();
  //const std::string outputFileName = vm["output-file"].as<std::string>();
  Long64_t nEntries = vm["num-events"].as<Long64_t>();

  // Initialize xAOD infrastructure
  xAOD::TReturnCode::enableFailure();
  CP::SystematicCode::enableFailure();
  CP::CorrectionCode::enableFailure();
  StatusCode::enableFailure();
  CHECK( xAOD::Init() );

  // Event storage
  xAOD::TEvent event;
  xAOD::TStore store;

  // Open and attach input file
  std::unique_ptr<TFile> inputFile( TFile::Open(inputFileName.c_str()) );
  CHECK( event.readFrom(inputFile.get()) );

  // Create and configure the QuickAna tool
  std::unique_ptr<ana::QuickAna> quickAna( new ana::QuickAna("quickana") );
  quickAna->eventinfoDef = "default";
  quickAna->electronDef = "default";
  quickAna->muonDef = "default";
  quickAna->photonDef = "default";
  quickAna->tauDef = "default";
  quickAna->jetDef = "default";
  quickAna->jetKine = "pt > 20e3";
  quickAna->metDef = "default";
  quickAna->orDef = "default";
  CHECK( quickAna->initialize() );

  // Prepare the systematics
  std::vector<CP::SystematicSet> sysList;
  if(vm.count("sys"))
    sysList = CP::make_systematics_vector( quickAna->recommendedSystematics() );
  else
    sysList.push_back(CP::SystematicSet());
  CHECK( quickAna->setSystematics(sysList) );

  // Number of entries to process
  Long64_t nEntriesAvail = event.getEntries();
  if(nEntries < 0) nEntries = nEntriesAvail;
  else nEntries = std::min(nEntries, nEntriesAvail);
  Info(appName, "Processing %lli events", nEntries);
  Long64_t entryPrintFreq = nEntries/10;

  const auto startTime = std::chrono::steady_clock::now();

  // Start Callgrind monitoring here if delayed
  CALLGRIND_START_INSTRUMENTATION;

  // Loop over the events
  for(Long64_t entry = 0; entry < nEntries; ++entry) {
    if((entry % entryPrintFreq) == 0)
      Info(appName, "Processing entry %lli", entry);
    event.getEntry(entry);

    // Loop over the systematics
    for(auto sys : quickAna->systematics()){
      CHECK( quickAna->applySystematicVariation(sys) );

      // Run QuickAna
      CHECK( quickAna->process(event) );
    }

    store.clear();
  }

  // End Callgrind monitoring here
  CALLGRIND_STOP_INSTRUMENTATION;

  // Report the average event rate
  const auto endTime = std::chrono::steady_clock::now();
  std::chrono::duration<float> duration = endTime - startTime;
  float evtTime = duration.count();
  //float evtTime = (float) (endTime - startTime) / std::chrono::seconds(1);
  float evtRate = (float) nEntries / evtTime;
  Info(appName, "Event loop finished");
  Info(appName, "Processing time: %.2f s", evtTime);
  Info(appName, "Average event rate: %.2f evts/s", evtRate);

  Info(appName, "Application finished");
  return EXIT_SUCCESS;
}
