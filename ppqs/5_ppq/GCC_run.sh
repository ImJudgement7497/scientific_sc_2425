#!/bin/bash

module purge 
module load compiler/GCC
gfortran -O3 test_inv.f90
./a.out >> ./results_c1/teaching0_GCC.txt
rm ./a.out
