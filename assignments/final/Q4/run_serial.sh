#!/bin/bash

make clean
make serial_debug
rm coords.txt
rm distances.txt
rm ./plots/*
./bin/main_serial_debug
echo "Plotting points now!"
python3 graphing/graph.py
