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

thread_scaling_times = read_data_from_file("./data/thread_scaling_times.txt")
speed_up = read_data_from_file("./data/speedup.txt")
thread_nums = np.array([1, 2, 3, 4, 8, 16, 32, 40, 48, 56, 64, 72, 80, 88, 96])
##########################
plt.plot(thread_nums, thread_scaling_times, "o-")
plt.title("Thread Scaling for Time")
plt.xlabel("Threads")
plt.ylabel("Time(s)")
plt.savefig("./data/thread_scaling_times.png")
plt.close()
##########################
plt.plot(thread_nums, speed_up, "o-")
plt.plot(thread_nums, thread_nums, "red")
plt.title("Speedup from 1 -> 96 threads")
plt.legend(["Actual Speedup", "Ideal Speedup"])
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.savefig("./data/speedup.png")
plt.close()