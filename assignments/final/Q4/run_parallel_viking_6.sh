#!/bin/bash

if [ -z "$1" ]; then
    echo "Error: Please provide a make rule as argument"
    echo "Usage: $0 <make_rule>"
    exit 1
fi

MAKE_RULE=$1

THREADS=("40" "48" "56")


L=$(grep -oP '^L\s*\K[0-9.]+$' ./config/config.txt)
r=$(grep -oP '^r\s*\K[0-9.]+$' ./config/config.txt)
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' ./config/config.txt)

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

    output_dir="L=${L}, r=${r}/sf=${sf}/job_id=${JOB_ID}"
    output_dir2="/threads=${NUM_THREADS}"

    mkdir -p "./results/parallel_results/$output_dir/$output_dir2"

    echo "Running with $NUM_THREADS threads and job ID $JOB_ID..."
    ./bin/main_$MAKE_RULE $JOB_ID

    mv *.bin "./results/parallel_results/$output_dir/$output_dir2"
done

mv num_of_circles_${JOB_ID}.txt "./results/parallel_results/$output_dir"
mv times_${JOB_ID}.txt "./results/parallel_results/$output_dir"

