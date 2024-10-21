#!/bin/bash

rm ./bin/main
make

# Loop from 1 to 12 to set OMP_NUM_THREADS
for num_threads in {1..20}
do
    # Export the number of threads to OMP_NUM_THREADS
    export OMP_NUM_THREADS=$num_threads
    
    # Print the current number of threads being used
    echo "Running main with OMP_NUM_THREADS=$num_threads"
    
    # Run the program
    ./bin/main >> "./results/($num_threads)_sum.txt"
    
done

