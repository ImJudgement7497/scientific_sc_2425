#!/bin/sh
#SBATCH --job-name=MINI                  # Job name
#SBATCH --output=./logs/output.log       # Standard out and error log
#SBATCH --mail-type=NONE                 # Specify when to mail (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=bmjm501@york.ac.uk   # Change UID if you want to send mail

# Need ONE of the following two lines:
#SBATCH --partition=teach                # Priority queue for class work
##SBATCH --account=dept-proj-year         # Specify project account if not doing class work

# Customise these according to job size and time required:
#SBATCH --cpus-per-task=10                # Each task uses 1 core
#SBATCH --time=00:05:00                  # Time limit hrs:min:sec

# Actual executable info now:
EXEC="./bin/main"

# Define a list of task counts to iterate over
TASK_COUNTS=(1 2 4 5 10)  # List the desired numbers of tasks

# Tell user what is going on
echo "My working directory is $(pwd)"
echo "Running job on host:"
echo -e '\t' "$(hostname) at $(date)"

# Loop through the task counts
for NTASKS in "${TASK_COUNTS[@]}"; do
    echo -e "\nRunning with ${NTASKS} tasks..."
    
    # Set SLURM_NTASKS for this run
    export SLURM_NTASKS=$NTASKS
    
    # Execute the run script
    ./runs/run_parallel.sh
    echo "Executing mpiexec with ${SLURM_NTASKS} tasks"
    mpiexec -n ${SLURM_NTASKS} $EXEC

    echo -e "\nCompleted run with ${NTASKS} tasks at $(date)"
done

echo
echo "All runs completed at $(date)"
