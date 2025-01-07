#!/bin/bash

# Ensure a make rule was provided as the first argument
if [ -z "$1" ]; then
    echo "Error: Please provide a make rule as argument"
    echo "Usage: $0 <make_rule> [--config=PATH_TO_CONFIG]"
    exit 1
fi

# Capture the make rule (first argument)
MAKE_RULE=$1
shift  # Shift the positional arguments so we can handle --config or other arguments

# Default thread values
THREADS=("1" "96")
#THREADS=("96")

# Default config path
CONFIG_PATH="./config/config.txt"  # Default config file location

# Parse remaining arguments, especially --config if it exists
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --config=*)  # Check if --config has been provided
            CONFIG_PATH="${1#--config=}"  # Get the config path after --config=
            shift
            ;;
        *) 
            echo "Unknown parameter passed: $1"
            echo "Usage: $0 <make_rule> [--config=PATH_TO_CONFIG]"
            exit 1
            ;;
    esac
done

# Read values from the configuration file
L=$(grep -oP '^L\s*\K[0-9.]+$' "$CONFIG_PATH")
r=$(grep -oP '^r\s*\K[0-9.]+$' "$CONFIG_PATH")
sf=$(grep -oP '^sampling_frequency\s*\K[0-9.]+$' "$CONFIG_PATH")

if [ -z "$L" ] || [ -z "$r" ] || [ -z "$sf" ]; then
    echo "Error: Could not find L, r, or sampling_frequency values in config file"
    exit 1
fi

# Ensure the SLURM job ID exists
JOB_ID=$SLURM_JOB_ID


if [ -z "$JOB_ID" ]; then
    echo "Error: SLURM job ID not found"
    exit 1
fi

# Clean and compile the make rule
make clean
make "$MAKE_RULE"

# Loop over the predefined list of threads
for NUM_THREADS in "${THREADS[@]}"; do
    export OMP_NUM_THREADS=$NUM_THREADS  # Set number of threads

    # Create output directories
    output_dir="L=${L}, r=${r}/sf=${sf}/job_id=${JOB_ID}"
    output_dir2="/threads=${NUM_THREADS}"

    mkdir -p "./results/parallel_results/$output_dir/$output_dir2"

    echo "Running with $NUM_THREADS threads and job ID $JOB_ID..."
    ./bin/main_"$MAKE_RULE" --config="$CONFIG_PATH" $JOB_ID  # Pass config argument to the binary

    # Move the generated binary files to the appropriate directory
    mv *.bin "./results/parallel_results/$output_dir/$output_dir2"
done

# Move result files to the correct location
mv num_of_circles_${JOB_ID}.txt "./results/parallel_results/$output_dir"
mv times_${JOB_ID}.txt "./results/parallel_results/$output_dir"
