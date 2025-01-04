#!/bin/bash

# Check if a make rule is provided as a parameter
if [ -z "$1" ]; then
    echo "Error: Please provide a make rule as an argument"
    exit 1
fi

MAKE_RULE=$1

# Load the values for L and r from the input file
L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' ./config/config.txt)

# Check if values for L and r are found
if [ -z "$L" ] || [ -z "$r" ] || [ -z "$sf" ]; then
    echo "Error: Could not find L, r or sampling freuqency values in config file"
    exit 1
fi

make clean
make $MAKE_RULE

output_dir="L=${L}, r=${r}"
output_dir2="sf=${sf}"
mkdir -p "./results/serial_results/$output_dir/$output_dir2"

time ./bin/main_$MAKE_RULE
echo "Plotting points now!"

python3 graphing/graph.py

mv *.txt *.bin ./plots/* "./results/serial_results/$output_dir/$output_dir2"
