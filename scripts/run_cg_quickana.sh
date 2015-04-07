#!/bin/bash

# Here's an example profiling command
valgrind --tool=callgrind --instr-atstart=no quickana_profile -i /afs/cern.ch/atlas/project/PAT/xAODs/r5591/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.recon.AOD.e1727_s1933_s1911_r5591/AOD.01494882._111853.pool.root.1 -n 10
