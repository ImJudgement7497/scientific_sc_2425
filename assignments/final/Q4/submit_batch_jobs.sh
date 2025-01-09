#!/bin/bash

JOB_SCRIPT="./job_parallel_ratio.sh"

# Check if the job script exists
if [[ ! -f $JOB_SCRIPT ]]; then
    echo "Error: $JOB_SCRIPT not found!"
    exit 1
fi

for NUM in {1..2}; do
    TEMP_SCRIPT="./temp_job_script_${NUM}.sh"
    sed "s/XT/${NUM}/g" "$JOB_SCRIPT" > "$TEMP_SCRIPT"
    echo "Submitting job with XT=${NUM}..."
    sbatch "$TEMP_SCRIPT"
    rm "$TEMP_SCRIPT"
done
