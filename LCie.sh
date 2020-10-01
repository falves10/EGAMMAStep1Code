#!/bin/bash

if [[ $# -ne 3 ]]; then
echo "USAGE: LCie.sh <jobname> <executable> <systematics>   <luminosity>"
else
jobname=$1
shift
#diffVar=$1
#shift
ijob=$1
shift
mc=$1



base_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/condor/Calib/"
output_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/results/Calib/"
run_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/run_tmp/Calib/"
source_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples"

if [ ! -e ${output_dir} ];then
echo "Can not make working directory ${output_dir} Exit"
exit
fi


echo ${run_dir}/"${jobname}_${ijob}"

work_dir=${run_dir}/"${jobname}_${ijob}"


rm -fr ${work_dir}
mkdir -vp ${work_dir}

if [ ! -e ${work_dir} ];then
echo "Can not make working directory ${work_dir}. Exit"
exit
fi
pushd ${work_dir}

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh
lsetup root
lsetup "root 6.20.06-x86_64-centos7-gcc8-opt"

cp ${base_dir}/${jobname}/exe/egamma_test_condor.C ./
cp ${base_dir}/${jobname}/exe/"$mc".list ./
 

a="condor"
echo "MC" $mc
root -l -b -q egamma_test_condor.C\(\"$mc\"\,\"$a\"\) > log 2>err

#########################################################################
popd
#    rm -fr ${work_dir}

echo "----------All done------------"
date
#    sleep 60			# it is always bad to be too fast for a man.

fi
