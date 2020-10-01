#!/bin/bash

channel=("ee" "mumu")
#channel=("ee" )
point=("140" "200")
#point=("140")
#cut=("0p15" "0p16" "0p17" "0p18" "0p19" "0p2" "0p21" "0p22" "0p23" "0p24" "0p25" "0p26" "0p27" "0p28" "0p29" "0p3" "0p31" "0p32" "0p33" "0p34" "0p35" "0p36" "0p37" "0p38" "0p39" "0p4" "0p5")

cut=("0p05" "0p1" "0p15" "0p17" "0p2")
#cut=("0p05" )
arr=("data")


if [ $arr == "data" ]
then
	selection=("CR1" "SignalRegionSelection")

elif [ $arr == "bkg" ]
then
	selection=("SignalRegionSelection")
fi

path=/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/ZyHM_versioWithORL/MyAnalysis/run/optmization/forCrossCheck/getCutflows/condor_submit/datarun/results
origin=/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/ZyHM_versioWithORL/MyAnalysis/run/optmization/forCrossCheck/getCutflows/condor_submit/
formerge=/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/ZyHM_versioWithORL/MyAnalysis/run/optmization/forCrossCheck/getCutflows/condor_submit/mergeResults

if [ $arr = "data" ]; then
for ch in "${channel[@]}";do #1
    for mass in "${point[@]}";do #2
        for sel in "${selection[@]}";do #3
            for icut in "${cut[@]}";do #4
                cd $path

                echo $ch $mass $sel $icut
            	
		hadd  result_data_"$ch"_"$mass"_"$sel"_"$icut".root   ./XZG_data15_"$mass"/root/ph_info_"$ch"_"$mass"_data15_"$sel"_"$icut".root ./XZG_data16_"$mass"/root/ph_info_"$ch"_"$mass"_data16_"$sel"_"$icut".root ./XZG_data17_"$mass"/root/ph_info_"$ch"_"$mass"_data17_"$sel"_"$icut".root ./XZG_data18_"$mass"/root/ph_info_"$ch"_"$mass"_data18_"$sel"_"$icut".root
		  
		done #4
        done #3
    done #2
done #1

mv *.root $formerge/

cd $origin

echo "finished merging"
fi



if [ $arr = "bkg" ]; then

process=("noMode_dataMC")

for ch  in "${channel[@]}"; do #loop in channel
    for p in "${point[@]}"; do #point in channel
        for sel in "${selection[@]}"; do #point in channel
            for proc in "${process[@]}"; do #point in channel
                for icut in "${cut[@]}"; do #point in channel
                echo $ch $p $sel $proc $icut 
                
		cd $formerge

		$PWD
                #merging MC16A/D/E (pTy)
                hadd bkg_pTy_"$ch"_"$p"_"$sel"_"$proc"_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16a_"$ch"gamma_LO_pty_*_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16d_"$ch"gamma_LO_pty_*_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16e_"$ch"gamma_LO_pty_*_"$icut".root
                
                #merging MC16A/D/E (M)
                hadd  bkg_M_"$ch"_"$p"_"$sel"_"$proc"_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16a_"$ch"gamma_M_*_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16d_"$ch"gamma_M_*_"$icut".root ph_info_"$ch"_"$p"_"$sel"_"$proc"_mc16e_"$ch"gamma_M_*_"$icut".root
                
                #merging final files
                hadd bkg_"$ch"_"$p"_"$sel"_"$proc"_"$icut".root bkg_pTy_"$ch"_"$p"_"$sel"_"$proc"_"$icut".root bkg_M_"$ch"_"$p"_"$sel"_"$proc"_"$icut".root

                done
            done
        done
    done
done

rm bkg_M*  bkg_pTy*
mv bkg_*.root $formerge/bkg/
rm *root
cd $origin

echo "finished merging"

fi


