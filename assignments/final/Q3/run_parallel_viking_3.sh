#!/bin/bash

THREADS=("3")

for NUM_THREADS in "${THREADS[@]}"; do
    export OMP_NUM_THREADS=$NUM_THREADS  # Set number of threads

    echo "Running with $NUM_THREADS threads..."
    ./bin/main_packing_parallel
    diff first_run/final_positions.txt final_positions.txt
    echo "Differences above (if any)"
    echo "-----------------------------------"
done


