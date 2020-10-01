#!/bin/bash

jobname=EGCal
inputname=WH
option=initial_weight
njob=20

#running MC variations and  MCnominal
for inputname in mc16A_2015_nom mc16A_2015_var mc16A_2016_nom mc16A_2016_var mc16D_2017_nom mc16D_2017_var mc16E_2018_nom mc16E_2018_var
#running data (with variations) (it can also be included to run together in the previous line if desired)
for inputname in data2015_var data2016_var  data2017_var  data2018_var 
do
echo $njob
sh split.sh  ${jobname}_${inputname} ${inputname}.list ${njob} ${inputname} 
done
