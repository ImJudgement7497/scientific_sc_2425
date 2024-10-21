import matplotlib.pyplot as plt
import numpy as np

with open("./results_2/parra_times_pc.txt", "r") as file:
    times = [float(line.strip()) for line in file]

x = np.arange(1, len(times) + 1, 1)

plt.plot(x, times)
plt.xticks(np.arange(1, len(times) + 1, 1))
plt.xlabel("Number of Threads")
plt.ylabel("Time (s)")
plt.title("Parallel speedup for N=1000000 iterations (PC)")
plt.grid()
plt.savefig("./results_2/times_pc.png")
