#!/bin/bash

module purge 
module load compiler/GCC
gfortran -O0 test_inv.f90
./a.out
rm ./a.out
