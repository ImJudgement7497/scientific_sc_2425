#!/bin/bash

module purge 
module load compiler/GCC
gfortran -Ofast test_inv.f90
./a.out >> ./results_c3/teaching0_GCC.txt
rm ./a.out
