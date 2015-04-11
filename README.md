# QuickAnaProfiling
A package for profiling QuickAna with valgrind

## Checkout the package
with git:

    git clone git git@github.com:sparticlesteve/QuickAnaProfiling.git

with svn:

    svn checkout https://github.com/sparticlesteve/QuickAnaProfiling/trunk QuickAnaProfiling

## Preparation
_Before_ you compile, you need to setup valgrind from CVMFS by doing

    localSetupSFT valgrind/3.8.0

This is needed because it sets up an environment variable that's used to locate the valgrind header files.

## Running

At the moment there's just one executable, which you can run with

    quickana_profile -i [input-file] -n [num-events] ...

Run with `-h` to see the full list of command line options. I'll add more functionality shortly.
