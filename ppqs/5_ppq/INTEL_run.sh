#!/bin/bash

module purge
module load compiler/intel-compilers 
ifort -O0 test_inv.f90
./a.out
rm ./a.out
