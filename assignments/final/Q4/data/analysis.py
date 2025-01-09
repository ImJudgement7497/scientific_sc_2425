import numpy as np
import matplotlib.pyplot as plt

def read_data_from_file(file_path):
    try:
        with open(file_path, "r") as file:
            data = [float(line.strip()) for line in file if line.strip()]
        return np.array(data)
    except FileNotFoundError:
        print(f"Error: File not found at {file_path}")
        return np.array([])
    except ValueError as e:
        print(f"Error: Unable to convert file contents to numbers. {e}")
        return np.array([])

circles_1_thread = read_data_from_file("./data/changing_r/ratio_1_thread_circles.txt")
circles_96_threads = read_data_from_file("./data/changing_r/ratio_96_thread_circles.txt")

times_1_thread = read_data_from_file("./data/changing_r/ratio_1_thread_times.txt")
times_96_threads = read_data_from_file("./data/changing_r/ratio_96_thread_times.txt")

r_l_ratios = np.arange(1, 21, 1) / 1000

differences = circles_96_threads - circles_1_thread
speedup = times_1_thread / times_96_threads

thread_scaling_circles = read_data_from_file("./data/thread_scaling/thread_scaling_circles.txt")
thread_scaling_times = read_data_from_file("./data/thread_scaling/thread_scaling_times.txt")
thread_scaling_speedup = read_data_from_file("./data/thread_scaling/thread_scaling_speedup.txt")
thread_nums = np.array([1, 2, 3, 4, 8, 16, 32, 40, 48, 56, 64, 72, 80, 88, 96])
##########################
plt.plot(r_l_ratios, speedup, "o-")
plt.title("Parallel Speedup from 1 -> 96 threads")
plt.xlabel("r/L")
plt.ylabel("Speedup")
plt.savefig("./data/changing_r/ratio_scaling_speedup.png")
plt.close()
##########################
plt.plot(r_l_ratios, differences, "o-")
plt.title("Num. of Circle difference from 1 -> 96 threads")
plt.xlabel("r/L")
plt.ylabel("Difference")
plt.savefig("./data/changing_r/ratio_scaling_differences.png")
plt.close()
##########################
plt.plot(thread_nums, thread_scaling_circles, "o-")
plt.title("Thread Scaling for Number of Circles")
plt.xlabel("Threads")
plt.ylabel("Num. of Circles")
plt.savefig("./data/thread_scaling/thread_scaling_circles.png")
plt.close()
##########################
plt.plot(thread_nums, thread_scaling_times, "o-")
plt.title("Thread Scaling for Time")
plt.xlabel("Threads")
plt.ylabel("Time(s)")
plt.savefig("./data/thread_scaling/thread_scaling_times.png")
plt.close()
##########################
plt.plot(thread_nums, thread_scaling_speedup, "o-")
plt.plot(thread_nums, thread_nums, "red")
plt.title("Speedup from 1 -> 96 threads")
plt.legend(["Actual Speedup", "Ideal Speedup"])
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.savefig("./data/thread_scaling/thread_scaling_speedup.png")
plt.close()