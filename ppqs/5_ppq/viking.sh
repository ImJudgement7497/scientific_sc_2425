#!/bin/bash

bash ~/scratch/scientific_sc_2425/pull.sh
sbatch job_GCC.sh
sbatch job_INTEL.sh
#bash ~/scratch/scientific_sc_2425/push.sh
