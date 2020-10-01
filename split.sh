#!/bin/bash

jobname=$1
inputname=$2
#option=$3
njob=$3
mc=$4

echo $jobname $inputname $njob

nfile=`wc -l < "$inputname"`

nfile_perjob=`expr $nfile / $njob`
nfile_lastjob=`expr $nfile % $njob`

if [ "$nfile_lastjob" -gt 0 ]; then
nfile_perjob=`expr $nfile_perjob + 1`
fi

base_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/condor/Calib/"
output_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/results/Calib/"
run_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/datarun/results/Calib/"
source_dir="/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/"

mkdir -vp ${base_dir}/${jobname}/job
mkdir -vp ${base_dir}/${jobname}/err
mkdir -vp ${base_dir}/${jobname}/log
mkdir -vp ${base_dir}/${jobname}/out
mkdir -vp ${base_dir}/${jobname}/exe
mkdir -vp ${base_dir}/${jobname}/input

mkdir -vp ${output_dir}/${jobname}/log
mkdir -vp ${output_dir}/${jobname}/err
mkdir -vp ${output_dir}/${jobname}/out
mkdir -vp ${output_dir}/${jobname}/root

rm -fr ${output_dir}/${jobname}/root/*root

#cp -fr ${source_dir}/../build/CreateH125yyBkg ${base_dir}/${jobname}/exe/

echo "split" ${inputname}

cp -fr ${inputname} ${base_dir}/${jobname}/exe/
cp -fr ${source_dir}/egamma_test_condor.C ${base_dir}/${jobname}/exe/
cp -fr LCie.sh  ${base_dir}/${jobname}/exe/
#cp -fr toberunFile.list  ${base_dir}/${jobname}/exe/
#########################################
ifile=0
ijob=0
while read line
do
ifile=`expr $ifile + 1`
tempfile=`expr $ifile % $nfile_perjob`


if [ "$nfile_perjob" -eq 1 ]; then
ijob=`expr $ijob + 1`
rm -fr "$jobname"_"$ijob".txt
echo "$line" > "$jobname"_"$ijob".txt
else
if [ "$tempfile" -eq 1 ]; then
ijob=`expr $ijob + 1`
rm -fr "$jobname"_"$ijob".txt
echo "$line" > "$jobname"_"$ijob".txt
else
echo "$line" >> "$jobname"_"$ijob".txt
fi
fi

###############################################
if [ "$tempfile" -eq 0 ]; then
mv  "$jobname"_"$ijob".txt  ${base_dir}/${jobname}/input/
################# script of qsub job  ################
jobfilename="${base_dir}/${jobname}/job/${jobname}_"$ijob".job"
rm -fr  ${jobfilename}
echo "#!/bin/bash"   > ${jobfilename}
echo " sh ${base_dir}/${jobname}/exe/LCie.sh  ${jobname}  ${ijob} ${mc}  > ${base_dir}/${jobname}/log/${jobname}_${ijob}.log 2>${base_dir}/${jobname}/err/${jobname}_${ijob}.err  "  >>  ${jobfilename}
chmod a+x ${jobfilename}


### HEP_SUB in IHEP
## To be finished (similar as QSUB)
#hep_sub -e ${base_dir}/${jobname}/err -o ${base_dir}/${jobname}/out ${jobfilename}
hep_sub -e err -o out ${jobfilename}
###

#for running locally
echo ${jobfilename}
#source ${jobfilename}

else
   if [ "$ifile" -eq "$nfile" ]; then
   mv  "$jobname"_"$ijob".txt  ${base_dir}/${jobname}/input/

##################### script of qsub job  ################
jobfilename="${base_dir}/${jobname}/job/${jobname}_"$ijob".job"
rm -fr  ${jobfilename}
echo "#!/bin/bash"   > ${jobfilename}
echo " sh ${base_dir}/${jobname}/exe/LCie.sh  ${jobname}   ${ijob} ${mc}  > ${base_dir}/${jobname}/log/${jobname}_${ijob}.log 2>${base_dir}/${jobname}/err/${jobname}_${ijob}.err  "  >>  ${jobfilename}
chmod a+x ${jobfilename}



### HEP_SUB in IHEP
## To be finished (similar as QSUB)
#hep_sub -e ${base_dir}/${jobname}/err -o ${base_dir}/${jobname}/out ${jobfilename}
hep_sub -e err -o out ${jobfilename}
###

#####################
   fi

fi


done <${inputname}
############################################
