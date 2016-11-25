#!/bin/bash

energies=("1 2 4 8 16 32 50 60")

for energy in $energies;
do
    ./simulate_hgcal --isInteractive=0 --energy=${energy} --particle=e- --numEvents=20000 --seed=1 --verbose=0 --geomConfigFile=hgc/config/config__SiW.geom --output=test__${energy}GeV.root > log__${energy}GeV.log &
done
