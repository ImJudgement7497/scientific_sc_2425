#!/bin/bash

module purge
module load compiler/intel-compilers 
ifort -Ofast test_inv.f90
./a.out >> ./results_c5/teaching0_INTEL.txt
rm ./a.out
