#!/bin/bash

# Define variables
VIKING_USER="bmjm501"
VIKING_HOST="viking.york.ac.uk"
VIKING_DIR="~/scratch/scientific_sc_2425/assignments/final/Q4"
CHECK_INTERVAL=60  # Time (in seconds) to wait between checks
REMOTE_SCRIPT="~/scratch/scientific_sc_2425/git_auto_commit.sh"  # Path to the auto script on VIKING

echo "Waiting for computations to finish on VIKING..."

while true; do
    # Check if computation has completed by looking for a specific "done" file
    ssh viking "[ -f $VIKING_DIR/done.txt ]"
    if [ $? -eq 0 ]; then
        echo "Computation finished. Executing auto script on VIKING..."
        
        # Run the auto script on VIKING
        ssh viking "bash $REMOTE_SCRIPT"
        
        break
    else
        echo "Computation still running. Checking again in $CHECK_INTERVAL seconds..."
        sleep $CHECK_INTERVAL
    fi
done
