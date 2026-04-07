#!/bin/bash

#split big file
split -a 4 -d -l 901 trajectory_total_step1000000_output_step100_run1.txt data_

for FILE in `ls data_*`
do
    awk '{if (NR==1) print $1}' $FILE > time.out
    read time < time.out
    
    rm incipit_*
    rm rest_*
    
    echo ITEM: TIMESTEP >> incipit_$FILE
    echo ${time} >> incipit_$FILE
    echo ITEM: NUMBER OF ATOMS >> incipit_$FILE
    echo 900 >> incipit_$FILE
    echo ITEM: BOX BOUNDS pp pp pp >> incipit_$FILE
    echo 0.0 52.8924880602 >> incipit_$FILE
    echo 0.0 17.6308293534 >> incipit_$FILE
    echo 0.0 1.0 >> incipit_$FILE
    echo ITEM: ATOMS id x y z c_strs[1] >> incipit_$FILE 

    tail -n +2 $FILE |  awk '{print NR,$0}' > rest_$FILE

    cat incipit_$FILE rest_$FILE > run1_mov_$FILE.dat

done
    
