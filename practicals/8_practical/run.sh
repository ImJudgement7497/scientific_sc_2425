#!/bin/bash

make clean
make 
mpirun -np 6 ./bin/main