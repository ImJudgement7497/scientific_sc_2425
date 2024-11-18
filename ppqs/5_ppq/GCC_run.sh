#!/bin/bash

module purge 
module load compiler/GCC
gfortran -O0 test_inv.f90
./a.out >> ./results_c4/teaching0_GCC.txt
rm ./a.out
