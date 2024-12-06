#!/bin/sh
#SBATCH --job-name=prac10                # Job name
#SBATCH --output=./logs/prac10_%j.log           # Standard out and error log
#SBATCH --mail-type=NONE                   # Specify when to mail (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=bmjm501@york.ac.uk         # NB change uid to your username if wanting to send mail

#need ONE of the following two lines:
#SBATCH --partition=gpu         # priority queue for class work
#SBATCH --account=pet-teach-2024           # specify your project account if NOT doing class work
#SBATCH --gres=gpu:1
#SBATCH --mem=8gb

#customise these according to job size and time required:
#SBATCH --ntasks=1                         # Run 4 MPI tasks...
#SBATCH --cpus-per-task=1                  # ...with each task using 1 core
#SBATCH --time=00:05:00                    # Time limit hrs:min:sec


module purge
module load CUDA/12.2.2

nvcc ./src/main.cu -o ./bin/main.x
nvidia-smi
nsys nvprof ./bin/main.x


echo
echo Job completed at `date`
