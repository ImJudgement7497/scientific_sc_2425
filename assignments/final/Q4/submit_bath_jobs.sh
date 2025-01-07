#!/bin/bash

# Path to the SLURM job script
JOB_SCRIPT="./job_parallel_ratio.sh"

# Check if the job script exists
if [[ ! -f $JOB_SCRIPT ]]; then
    echo "Error: $JOB_SCRIPT not found!"
    exit 1
fi

# Loop through numbers 1 to 20
for NUM in {1..20}; do
    # Create a temporary modified job script
    TEMP_SCRIPT="./temp_job_script_${NUM}.sh"

    # Replace "XT" with the current number
    sed "s/XT/${NUM}/g" "$JOB_SCRIPT" > "$TEMP_SCRIPT"

    # Submit the modified script
    echo "Submitting job with XT=${NUM}..."
    sbatch "$TEMP_SCRIPT"

    # Optionally delete the temporary script to keep the directory clean
    rm "$TEMP_SCRIPT"
done
