#!/bin/bash

bash ~/pull_from_git.sh
./GCC_run.sh
./INTEL_run.sh
bash ~/push_to_git.sh

