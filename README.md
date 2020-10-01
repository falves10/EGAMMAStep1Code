# EGAMMAStep1Code
This is the step1 code for filling the E/p and M(ee) distributions using the HTcondor service in IHEP clusters.

README:
For job summiting on IHEP HT condor, see the instructions below:
In your home directory, setup the HTcondor command: 

++++++++++++++++++++++++++++++++++++++++++++

export PATH=/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin:$PATH
++++++++++++++++++++++++++++++++++++++++++++

1) Create an directory named EG_condor and clone this repository in it:
git clone https://github.com/falves10/EGAMMAStep1Code.git

mkdir datarun

cd datarun

mkdir condor results run_tmp

cd ..

2) run.sh file: check whether you want to run variations and/or MC nominal or both commenting the pointed lines 

3) split.sh file: 

Change the path of the directories below accordingly:

base_dir="your_path/EG_condor/datarun/condor/"

output_dir="your_path/EG_condor/datarun/results/"

run_dir="your_path/EG_condor/datarun/results/"

source_dir="your_path/EG_condor/"

The following lines in this script are in charge for producing the jobs and submitting it:
hep_sub -e err -o out ${jobfilename}
If you want to test it locally, disable the previous line where it appears and enable the following line with the command 'for running locally':
source ${jobfilename} 

4) LCie.sh
Change the path of the directories below accordingly:

base_dir="your_path/EG_condor/datarun/condor/"

output_dir="your_path/EG_condor/datarun/results/"

run_dir="your_path/EG_condor/datarun/run_tmp/"

source_dir="your_path/EG_condor/"

5) egamma_test_condor.C: 
Enable or disable the variable you want to run on in the beginning of the code: varname vector
If you want to run using the layer1/2 electron calibrated energy vectors, uncomment it where it appears 
In the function mySave, change the name of the directory you would like to save the results

After performing the changes described previosly, you can launch the production as: sh run.sh 
