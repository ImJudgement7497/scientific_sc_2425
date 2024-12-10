#!/bin/bash

# Load the values for L and r from the input file
L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)

# Check if values for L and r are found
if [ -z "$L" ] || [ -z "$r" ]; then
    echo "Error: Could not find L or r values in config file"
    exit 1
fi

make clean
make serial_debug

output_dir="L=${L}, r=${r}"
mkdir -p "./results/$output_dir" 

./bin/main_serial_debug

echo "Plotting points now!"

python3 graphing/graph.py

mv coords.txt distances.txt ./plots/* "./results/$output_dir"
