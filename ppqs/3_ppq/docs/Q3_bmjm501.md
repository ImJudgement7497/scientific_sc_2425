## PPQ 3
**General Note**

The only things as required will be submitted as `Q3_bmjm501_main.cpp` and `Q3_bmjm501.pdf`, but a full TAR file called `Q3_bmjm501.tar` will be submitted alongside for a full look at the project, however is not required as this PDF answers all questions. I make references to files in the wider project, but again is not necessary to understand what I have done. (Further note, sorry for the poor formatting, I used markdown to make writing easier but I am unsure on how to convert it into a PDF without bad formatting. It is however all readable.)

**What is the value of sum for $N=10^{6}$?**

As can be seen in `results`, I have consistent results for the sums across a different number of `OMP_NUM_THREADS`

For $N=10^{6}$, I got a value of `500002285262.390808`

A preview of the sum data for 12 threads is below:

@import "../results/(12)_sum.txt"

**Parallel Speedup**

The graphs below are the parallel speedup from 1 to 20 threads on three different machines. My Home PC (6 cores), Teaching0 and VIKING.


@import "../results/times_HomePC.png"
@import "../results/times_Teaching0.png"
@import "../results/times_VIKING.png"

In general, we can see across the machines we are getting exponetial speedup as the number of threads increase. At the lower thread counts, the decrease is more rapid as each new thread can be assigned to it's own core, which may have hyperthreading built into it. However as the number of threads equals the hardware limit, we start to see diminishing returns. Past a certain point, we get small bumps which is likely due to background noise on the machine, particulary for when I am exceeding the amount of cores on the Home PC. Further Teaching0 had a lot of noise, but a rough exponetial shape is formed. Note these times per thread was the average of 100 runs of the function. This can be seen in `src/Q3_bmjm501_main.cpp: analysis()` or below in the code lisitng.

Specifically for the Home PC, there is a ~1.94x speedup between 1 to 2 threads, and ~1.46x speedup between 2 to 3 threads.

**Code listing**

The main source file can be found in `src/Q3_bmjm501_main.cpp`, however a listing with explaining comments is below:

@import "../src/Q3_bmjm501_main.cpp"

With other scripts like `analysis/graph.py`, `bin/get_times.sh` and `ppq3_viking.sh` used for graphing, doing shell commands to change the number of threads and running on VIKING respectivly.

It is important to note a modification that I made to the general algorithm. Instead of doing:
```c++
A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;
double y = A[i] - local_compensation;
double t = local_sum + y;
local_compensation = (t - local_sum) - y;
local_sum = t;
```

i.e. summing both the integer part `A[i]` and the smaller double part given by the `sqrt` expression, the resulting sum for $N=10^6$ was not consistent over different numbers of threads. To fix this, I instead implemented:
```c++
double y = sqrt(abs(sin(B[i]))) * 2.34 - local_compensation;
double t = local_sum + y;
local_compensation = (t - local_sum) - y;
local_sum = t;
// more code
double total_sum = kahan_sum(thread_sums);
total_sum = total_sum + 0.5 * N * (N + 1);
```

i.e only apply the Kahan summation algorithm to the small double part then add on the integer sum value using the expression $0.5N(N+1)$. This of course is mathematically the same and this fix then gave consistent results over all threads. Notably however, the only non-matching sums before the fix were due to the $N=1000000$ and $N=2000000$ term, for which I do not have an explanation for. 

