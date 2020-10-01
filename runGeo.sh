#!/bin/bash
  
#geo=("A" "EL" "IBL")
#geo=("A" "EL" "FMX" "IBL" "N" "PP0")
#geo=("FMX" "N" "PP0")
#geo=("NominalStudy")
geo=("condor")

#mc="mc16A"

for a in "${geo[@]}"; do
        echo $a
        mkdir $a
	cd $a
	mkdir rootFiles pdfs
	cd -
	
	export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
	source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh

	lsetup root 
	
	lsetup "root 6.14.04-x86_64-slc6-gcc62-opt" 
		
	#root -l -b -q invariantMassFitCal2.C\(\"$a\"\)
	#root -l -b -q teste_Egamma.C\(\"$a\"\)
	root -l -b -q egamma_test_condor.C\(\"$mc\"\,\"$a\"\)
done
