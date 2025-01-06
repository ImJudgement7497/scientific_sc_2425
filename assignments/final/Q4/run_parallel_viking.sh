#!/bin/bash

# Check if the correct arguments are passed
if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
    echo "Error: Please provide a make rule, start thread, and end thread as arguments"
    echo "Usage: $0 <make_rule> <start_threads> <end_threads>"
    exit 1
fi

MAKE_RULE=$1
START_THREADS=$2
END_THREADS=$3

# Validate that start thread is less than or equal to end thread
if [ "$START_THREADS" -gt "$END_THREADS" ]; then
    echo "Error: Start threads must be less than or equal to end threads."
    exit 1
fi

# Get values for L, r, and sampling frequency from config file
L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' ./config/config.txt)

# Check if values for L, r, and sf are found
if [ -z "$L" ] || [ -z "$r" ] || [ -z "$sf" ]; then
    echo "Error: Could not find L, r, or sampling_frequency values in config file"
    exit 1
fi

# Clean and compile
make clean
make $MAKE_RULE

# Loop from start to end thread counts
for NUM_THREADS in $(seq $START_THREADS $END_THREADS); do
    export OMP_NUM_THREADS=$NUM_THREADS  # Set number of threads

    # Define output directories based on the current number of threads
    output_dir="L=${L}, r=${r}/sf=${sf}"
    output_dir2="/threads=${NUM_THREADS}"

    # Create the necessary directories
    mkdir -p "./results/parallel_results/$output_dir/$output_dir2"

    # Run the binary with the current number of threads
    echo "Running with $NUM_THREADS threads..."
    ./bin/main_$MAKE_RULE

    # Move output files to the results folder
    mv data.txt *.bin "./results/parallel_results/$output_dir/$output_dir2"
    # mv num_of_circles.txt "./results/parallel_results/$output_dir"
    # mv times.txt "./results/parallel_results/$output_dir"
done

echo "Execution completed for all thread configurations from $START_THREADS to $END_THREADS."
