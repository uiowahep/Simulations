#!/bin/bash

export SIMULATIONSHOME=`dirname ${BASH_SOURCE[0]}`/..
echo "SIMULATIONS HOME is now at $SIMULATIONSHOME"

# for cern batch submission to have geant4 available.
# geant4 checks for some vars to be enabled internally
source /afs/cern.ch/sw/lcg/external/geant4/10.2.p01/x86_64-slc6-gcc49-opt-MT/bin/geant4.sh
