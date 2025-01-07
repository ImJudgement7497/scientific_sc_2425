#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Error: Please provide a make rule and number of threads as arguments"
    echo "Usage: $0 <make_rule> <num_of_threads>"
    exit 1
fi

MAKE_RULE=$1
NUM_THREADS=$2

export OMP_NUM_THREADS=$NUM_THREADS

L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' ./config/config.txt)

if [ -z "$L" ] || [ -z "$r" ] || [ -z "$sf" ]; then
    echo "Error: Could not find L, r, or sampling_frequency values in config file"
    exit 1
fi

make clean
make $MAKE_RULE

output_dir="L=${L}, r=${r}/sf=${sf}"
output_dir2="/threads=${NUM_THREADS}"
mkdir -p "./results/parallel_results/$output_dir/$output_dir2"

./bin/main_$MAKE_RULE

mv data.txt num_of_circles.txt time.txt *.bin  "./results/parallel_results/$output_dir/$output_dir2"


