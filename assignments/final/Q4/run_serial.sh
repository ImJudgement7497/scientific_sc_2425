#!/bin/bash

make clean
make serial
./bin/main_serial
python3 graphing/graph.py
