#!/bin/bash

ifile="/afs/cern.ch/atlas/project/PAT/xAODs/r5591/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.recon.AOD.e1727_s1933_s1911_r5591/AOD.01494882._111853.pool.root.1"

# Here's an example profiling command
# The instr-atstart command tells callgrind not to start until specified by the code.
# The smc-check is needed for callgrind to handle ROOT 6 modifying code.
valgrind --tool=callgrind --instr-atstart=no --smc-check=all-non-file quickana_profile -i $ifile -n 10
