import matplotlib.pyplot as plt
import numpy as np

with open("./results/parra_times_teaching0.txt", "r") as file:
    times = [float(line.strip()) for line in file]

x = np.arange(1, len(times) + 1, 1)

plt.plot(x, times)
plt.xticks(np.arange(1, len(times) + 1, 1))
plt.xlabel("Number of Threads")
plt.ylabel("Time (s)")
plt.title("Parallel speedup for N=1000000 iterations (Home PC)")
plt.grid()
plt.savefig("./results/times_teaching0.png")