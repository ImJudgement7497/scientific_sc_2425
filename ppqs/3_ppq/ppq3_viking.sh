#!/bin/sh
#SBATCH --job-nameppq3                # Job name
#SBATCH --output=./logs/PPQ3.log           # Standard out and error log
#SBATCH --mail-type=NONE                   # Specify when to mail (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=bmjm501@york.ac.uk         # NB change uid to your username if wanting to send mail

#need ONE of the following two lines:
#SBATCH --partition=teach         # priority queue for class work
##SBATCH --account=dept-proj-year           # specify your project account if NOT doing class work

#customise these according to job size and time required:
#SBATCH --ntasks=1                         # Run a single task...
#SBATCH --cpus-per-task=20                  # ...with four cores
#SBATCH --time=00:05:00                    # Time limit hrs:min:sec

#actual executable info now:
EXEC="./bin/main"

#load modules
module load matplotlib/3.8.2-gfbf-2023b 
#tell user what is going on:

echo My working directory is `pwd`
echo Running job on host:
echo -e '\t'`hostname` at `date`
echo -e '\t'with $SLURM_CPUS_ON_NODE CPU cores available
echo -e '\t'using $OMP_NUM_THREADS OMP threads
echo

bash ./bin/update_machine.sh

echo
echo Job completed at `date`
