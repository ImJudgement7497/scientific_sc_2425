#!/bin/bash

module purge
module load compiler/intel-compilers 
ifort -O0 test_inv.f90
./a.out >> ./results_b/teaching0_INTEL.txt
rm ./a.out
