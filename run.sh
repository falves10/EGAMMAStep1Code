#!/bin/bash


declare datasetname=0
function get_dateasetname(){
  case $1 in
    "ggH" ) datasetname="mc12_8TeV.160010.PowhegPythia8_AU2CT10_ggH130_gamgam.merge.NTUP_PHOTON.e1189_s1771_s1741_r4829_r4540_p1344";;  ## updown
    "VBF"  )  datasetname="mc12_8TeV.160025.PowhegPythia8_AU2CT10_VBFH130_gamgam.merge.NTUP_PHOTON.e1195_s1771_s1741_r4829_r4540_p1344";;  ## updown
    "ttH" ) datasetname="mc12_8TeV.160070.Pythia8_AU2CTEQ6L1_ttH130_gamgam.merge.NTUP_PHOTON.e1189_s1771_s1741_r4829_r4540_p1344";;    ## sysmetry
    "WH" ) datasetname="mc12_8TeV.160040.Pythia8_AU2CTEQ6L1_WH130_gamgam.merge.NTUP_PHOTON.e1189_s1771_s1741_r4829_r4540_p1344";;             ## sysmetry
    "ZH" ) datasetname="mc12_8TeV.160055.Pythia8_AU2CTEQ6L1_ZH130_gamgam.merge.NTUP_PHOTON.e1189_s1771_s1741_r4829_r4540_p1344";;          ## updown
    "data" ) datasetname="data12_8TeV.00200987.physics_Egamma.merge.NTUP_PHOTON.r4065_p1278_p1341_p1344_p1345";;          ## updown

    esac
}

jobname=EGCal
inputname=WH
option=initial_weight
njob=20

#for inputname in   mc16A_nom_1 #mc16A_var_1 mc16A_var_2 mc16A_nom_1 mc16A_nom_2 mc16D_var mc16D_nom mc16E_var_1 mc16E_var_2  mc16E_nom
for inputname in mc16A_2015_nom mc16A_2015_var mc16A_2016_nom mc16A_2016_var mc16D_2017_nom mc16D_2017_var mc16E_2018_nom mc16E_2018_var
#for inputname in  mc16D_2017_nom mc16D_2017_var mc16E_2018_nom mc16E_2018_var
#for inputname in  mc16A_2015_var mc16A_2016_var mc16D_2017_var mc16E_2018_var
#for inputname in data2015_var data2016_var  data2017_var  data2018_var 
do
#for diffVar in pT_yydo
echo $njob
#sh split.sh  ${jobname}_${inputname}_${diffVar} ${inputname}.list ${diffVar} ${njob} 
sh split.sh  ${jobname}_${inputname} ${inputname}.list ${njob} ${inputname} 
#done
done
