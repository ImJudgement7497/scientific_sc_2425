#!/bin/bash

ssh viking << `EOF`
cd scratch/scientific_sc_2425/practicals/10_practical
git pull
sbatch job.sh
EOF