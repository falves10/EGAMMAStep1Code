#!/bin/bash
#eta=(0 1 2 3 4 5 6 7)
#energy=(0 1 2 3 4 5 6 7 8 9)
eta=(0 1 2 3 4 5 6 7)
#eta=(5 6 7)
#eta=(3 4 5 6 7)
#eta=(0 1 2 3)
#eta=(4 5 6 7)
#eta=(10 11 12)#eta=(0 1 2 3 4 5 6 7 8 9)
#eta=(0 1 2 3 4 5 6 7)
#energy=(7 8 9)
#energy=(1)
energy=(0 1 2 3 4 5 6 7 8 9)
#energy=(7 8 9)
#energy=(0 1 2 3 4)
#energy=(5 6 7 8 9)
#energy=(0 1 2 3 4 5)
bias=(-1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17)
#bias=(-1 7)
#bias=(-1)

for i in "${eta[@]}";do
        for j in "${energy[@]}"; do
		for k in "${bias[@]}"; do
		echo " eta,  energy and bias " $i $j $k
		root -l -b  <<EOF 
	
                .x test.cxx("$i","$j","$k") 
EOF
		done
	done
done




#!/bin/bash

#l1=(200 300 500 1000 2000 2500 3500 4000 4500)
#l2=(0.05 0.10 0.20 0.30 0.40 0.50)

#l2=(0.05)

#for i in "${l1[@]}"; do
#        for j in "${l2[@]}"; do
                #echo $i $j

#        root -l <<EOF 

#                .L summing.cxx
#                summing("$i", "$j")
#EOF



#        done > finalCutflow_"$i".txt
#done  #> finalCutflow_$i.txt

