## PPQ 3
**General Note**

The only things as required will be submitted as `Q3_bmjm501_main.cpp` and `Q3_bmjm501.pdf`, but a full TAR file called `Q3_bmjm501.tar` will be submitted alongside for a full look at the project, however is not required as this PDF answers all questions.

**What is the value of sum for $N=10^{6}$?**

As can be seen in `results/2_threads.txt`, `results/4_threads.txt` and `results/12_threads.txt`, I have consistent results for the sums across a different number of `OMP_NUM_THREADS`

For $N=10^{6}$, I got a value of `500002285262.390808`

A preview of `12_threads.txt` is below:

@import "../results/12_threads.txt"

**Parallel Speedup**

The graphs below are the parallel speedup from 1 to 20 threads on three different machines. My Home PC (6 cores), Teaching0 and VIKING.


@import "../results/times.png"
@import "../results/times_teaching0.png"
@import "../results/times_VIKING.png"

In general, we can see across the machines we are getting exponetial speedup as the number of threads increase. At the lower thread counts, the decrease is more rapid as each new thread can be assigned to it's own core, which may have hyperthreading built into it. However as the number of threads equals the hardware limit, we start to see diminishing returns. Past a certain point, we get small bumps which is likely due to background noise on the machine, particulary for when I am exceeding the amount of cores on the Home PC. Note these times per thread was the average of 100 runs of the function. This can be seen in `src/Q3_bmjm501_main.cpp: analysis()` or below in the code lisitng.

Specifically for the Home PC, there is a ~1.96x speedup between 1 to 2 threads, and ~1.47x speedup between 2 to 3 threads.

**Code listing**

The main source file can be found in `src/Q3_bmjm501_main.cpp`, however a listing with explaining comments is below:

@import "../src/Q3_bmjm501_main.cpp"

With other scripts like `analysis/graph.py`, `bin/get_times.sh` and `ppq3_viking.sh` used for graphing, doing shell commands to change the number of threads and running on VIKING respectivly.

