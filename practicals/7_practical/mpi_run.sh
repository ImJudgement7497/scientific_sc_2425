#!/bin/bash

make
mpirun -np 6 ./bin/main
make clean