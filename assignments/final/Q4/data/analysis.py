import numpy as np
import matplotlib.pyplot as plt

times_1_thread = np.array([
    610.832874, 299.347380, 142.934938, 134.451639, 105.762529, 42.947283,
    31.434714, 24.935044, 21.236937, 18.573556, 17.048064, 4.771479, 3.767288,
    2.240967, 0.569546
])

times_96_threads = np.array([
    18.910452, 11.403005, 10.990958, 7.671533, 5.563378, 4.737450,
    3.662494, 3.125215, 2.895760, 2.759496, 2.725282, 2.057621, 1.328654,
    1.083615, 0.684059
])

circles_1_thread = np.array([
    166986, 106641, 92355, 59432, 41159, 25960, 22625, 17869, 
    14409, 11366, 9971, 6223, 4301, 2393, 980
])

circles_96_threads = np.array([
    166815, 106327, 93457, 59455, 41029, 26097, 22840, 17953, 
    14457, 11449, 9795, 6328, 4275, 2385, 1024
])

mean_circles_thread_scaling = np.array([26703,
    26646, 26830, 26713, 26770, 26715,  26798,  26860,  26745,
    26664, 26740, 26869, 26757
])

mean_times_thread_scaling = np.array([507.266506,
122.714003,
115.333433,
86.598033,
80.061899,
65.159961,
60.394195,
57.760489,
48.780394,
42.371885,
42.234132,
46.167095,
38.491898
])

thread_nums = np.array([1, 8, 16, 24, 32, 40, 48, 56, 64,
                        72, 80, 88, 96])

r_l_ratios_numbers = [
    1/1000, 1/800, 1/750, 1/600, 2/1000, 2/800, 
    2/750, 3/1000, 2/600, 3/800, 3/750, 3/600, 
    3/500, 2/250, 3/250
]

difference = circles_96_threads - circles_1_thread

speedup = times_1_thread / times_96_threads

plt.plot(r_l_ratios_numbers, speedup, "o-")
plt.title("Parallel Speedup from 1 -> 96 threads")
plt.xlabel("r/L")
plt.ylabel("Speedup")
plt.savefig("./data/ratio_scaling_speedup.png")
plt.close()
##########################
plt.plot(r_l_ratios_numbers, difference, "o-")
plt.title("Num. of Circle difference from 1 -> 96 threads")
plt.xlabel("r/L")
plt.ylabel("Difference")
plt.savefig("./data/ratio_scaling_differences.png")
plt.close()
##########################
plt.plot(thread_nums, mean_circles_thread_scaling, "o-")
plt.title("Thread Scaling for Mean Number of Circles")
plt.xlabel("Threads")
plt.ylabel("Mean Num. of Circles")
plt.savefig("./data/thread_scaling_mean_circles.png")
plt.close()
##########################
plt.plot(thread_nums, mean_times_thread_scaling, "o-")
plt.title("Thread Scaling for Mean Time")
plt.xlabel("Threads")
plt.ylabel("Mean Time(s)")
plt.savefig("./data/thread_scaling_mean_times.png")
plt.close()