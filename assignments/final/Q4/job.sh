#!/bin/sh
#SBATCH --job-name=Q4                # Job name
#SBATCH --output=./logs/Q4.log           # Standard out and error log
#SBATCH --mail-type=END                   # Specify when to mail (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=bmjm501@york.ac.uk         # NB change uid to your username if wanting to send mail

#need ONE of the following two lines:
#SBATCH --partition=teach         # priority queue for class work
##SBATCH --account=dept-proj-year           # specify your project account if NOT doing class work

#customise these according to job size and time required:
#SBATCH --ntasks=1                         # Run 4 MPI tasks...
#SBATCH --cpus-per-task=1                  # ...with each task using 1 core
#SBATCH --time=01:00:00                    # Time limit hrs:min:sec


bash ./run_serial.sh serial

echo
echo Job completed at `date`
