#!/bin/bash

if [ -z "$1" ]; then
    echo "Error: Please provide a make rule as argument"
    echo "Usage: $0 <make_rule>"
    exit 1
fi

MAKE_RULE=$1

# Define the fixed list of thread counts inside the script
THREADS=("1" "8" "16" "32" "40" "48")  # Add or remove thread numbers as needed

# Get L, r, and sampling frequency from config file
L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' ./config/config.txt)

# Check if values for L, r, and sf are found
if [ -z "$L" ] || [ -z "$r" ] || [ -z "$sf" ]; then
    echo "Error: Could not find L, r, or sampling_frequency values in config file"
    exit 1
fi

JOB_ID=$SLURM_JOB_ID
if [ -z "$JOB_ID" ]; then
    echo "Error: SLURM job ID not found"
    exit 1
fi

# Clean and compile
make clean
make $MAKE_RULE

# Loop over the predefined list of threads
for NUM_THREADS in "${THREADS[@]}"; do
    export OMP_NUM_THREADS=$NUM_THREADS  # Set number of threads

    # Define output directories based on the current number of threads
    output_dir="L=${L}, r=${r}/sf=${sf}/job_id=${JOB_ID}"
    output_dir2="/threads=${NUM_THREADS}"

    # Create the necessary directories
    mkdir -p "./results/parallel_results/$output_dir/$output_dir2"

    # Run the binary with the current number of threads and job ID passed as an argument
    echo "Running with $NUM_THREADS threads and job ID $JOB_ID..."
    ./bin/main_$MAKE_RULE $JOB_ID

    # Move output files to the results folder
    mv data.txt *.bin "./results/parallel_results/$output_dir/$output_dir2"
done

mv num_of_circles_${JOB_ID}.txt "./results/parallel_results/$output_dir"
mv times_${JOB_ID}.txt "./results/parallel_results/$output_dir"

echo "Execution completed for all thread configurations from $START_THREADS to $END_THREADS."
