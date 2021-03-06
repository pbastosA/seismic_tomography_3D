#!/bin/bash

model="src/essentials/model.cpp"
utils="src/essentials/utils.cpp"
geom="src/essentials/geometry.cpp"
eikonal="src/eikonal/eikonal.cpp"
tomography="src/tomography/tomography.cpp"

main="src/tomography/tomography_main.cpp"

essentials="$model $utils $geom"
paramFile="inputs/parameters.txt"
flags="-acc -fast -ta=tesla,cc60 -g -std::c++11 -fopenmp -lm"

target="bin/tomography3D.x" 

pgc++ $flags $main $eikonal $tomography $essentials -o $target   

rm *.o

./$target $paramFile

