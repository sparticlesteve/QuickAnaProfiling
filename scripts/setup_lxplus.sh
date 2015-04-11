# Source this script on lxplus to setup the latest valgrind version.
# I'm not sure how to do this more generally. My hope is that we can
# get a more recent valgrind version in SFT on CVMFS.
QuickAna_Valgrind_home=/afs/cern.ch/sw/lcg/releases/LCG_74root6/valgrind/3.10.0/x86_64-slc6-gcc48-opt
PATH=$QuickAna_Valgrind_home/bin:$PATH
export VALGRIND_LIB=$QuickAna_Valgrind_home/lib/valgrind

# Mimic the SFT env variable used in this package's Makefile
SFT_HOME_valgrind=$QuickAna_Valgrind_home
