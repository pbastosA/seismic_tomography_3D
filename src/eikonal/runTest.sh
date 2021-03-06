#!/bin/bash

python3 simbgf2022.py 1

model="../essentials/model/model.cpp"
utils="../essentials/utils/utils.cpp"
geom="../essentials/geometry/geometry.cpp"
eikonal="eikonal.cpp"

flags="-fopenmp -fast -acc -ta=tesla,cc60 -std=c++11 -g -lm"

pgc++ $utils $model $geom $eikonal eikonalAccuracyStudy.cpp $flags -o test.exe

./test.exe

rm *.o *.exe

for s in 1 2 3 4
do
    for i in 100 50 25
    do
        if [ $s = "1" ]; then
            mv "fim_central_${i}m_times_nr5048_shot_${s}.bin" "fim_5_${i}m.bin"
            mv "pod_central_${i}m_times_nr5048_shot_${s}.bin" "pod_5_${i}m.bin"
            mv "fsm_central_${i}m_times_nr5048_shot_${s}.bin" "fsm_5_${i}m.bin"
        fi
        
        mv "fim_extern_${i}m_times_nr5048_shot_${s}.bin" "fim_${s}_${i}m.bin"    
        mv "pod_extern_${i}m_times_nr5048_shot_${s}.bin" "pod_${s}_${i}m.bin"    
        mv "fsm_extern_${i}m_times_nr5048_shot_${s}.bin" "fsm_${s}_${i}m.bin"    
    done
done

python3 simbgf2022.py 0

rm *.bin *.txt
