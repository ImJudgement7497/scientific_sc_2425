import matplotlib.pyplot as plt
import numpy as np
import math
import struct

# ------------------------------I/O FUNCTIONS------------------------------ #
def load_coordinates(filename):
    with open(filename, "rb") as file:
        coordinates = []
        while True:
            byte_pair = file.read(2 * struct.calcsize('d')) 
            if not byte_pair: 
                break
            x, y = struct.unpack('dd', byte_pair)
            coordinates.append((x, y))
    return coordinates

def load_p_fractions(filename):
    with open(filename, "rb") as file:
        p_fractions = []
        while True:
            byte_data = file.read(struct.calcsize('d')) 
            if not byte_data: 
                break
            p_fraction = struct.unpack('d', byte_data)[0]
            p_fractions.append(p_fraction)
    return p_fractions

def load_config(filename):
    config = {}
    
    try:
        with open(filename, 'r') as file:
            for line in file:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                key, value = line.split()
                value = float(value)
                config[key] = value
        
        return config
    
    except FileNotFoundError:
        print(f"Error: Could not open file {filename}")
        return None
    except ValueError as e:
        print(f"Error parsing the file {filename}: {e}")
        return None

# ------------------------------PLOTTING FUNCTIONS------------------------------ #

def plot_circles(centers, radius, L):
    fig, ax = plt.subplots()
    for (x, y) in centers:
        circle = plt.Circle((x, y), radius, color='blue', fill=True, linewidth=0.001) # Make this a user parametr
        ax.add_patch(circle)

    ax.set_aspect('equal', adjustable='box')
    ax.set_xlim((0, L))
    ax.set_ylim(0, L)

    plt.grid(False)
    plt.title(f"Circles In Box, L = {L}, r = {radius}")
    plt.savefig("./plots/Circles_in_Box.png", dpi=300)
    plt.close()

def plot_points_in_box(centers, radius, L):
    box_limit = 0.025 * L
    filtered_points = [(x, y) for (x, y) in centers if 0 <= x <= box_limit and 0 <= y <= box_limit]

    fig, ax = plt.subplots()
    for (x, y) in centers:
        circle = plt.Circle((x, y), radius, color='blue', fill=True, linewidth=0.0001) # Make this a user parametr
        ax.add_patch(circle)

    ax.set_aspect('equal', adjustable='box')
    ax.set_xlim(0, box_limit)
    ax.set_ylim(0, box_limit)

    plt.grid(False)
    plt.title(f"Circles in Smaller Box, L = {L}, r = {radius}")
    plt.savefig("./plots/Circles_in_Smaller_Box.png", dpi=300)
    plt.close()

def plot_p_fractions(p_fractions, sampling_frequency):
    x = np.arange(0, len(p_fractions) * (sampling_frequency // 4), step=sampling_frequency // 4)
    plt.figure(figsize=(10, 6))
    plt.plot(x, p_fractions, marker='o', linestyle='-', color='b', label='Convergence')
    plt.title("Convergence of P with number of trials")
    plt.xlabel(f"Number of trials * sampling_frequency")
    plt.ylabel("log(Packing Fraction)")
    plt.yscale('log')
    plt.grid(False)
    plt.legend()
    plt.savefig("./plots/p_fraction_convergence.png", dpi=300)

# ------------------------------PLOTTING------------------------------ #
config = load_config("./config/config.txt")

if config:
    L = config.get('L')
    r = config.get('r')
    sampling_frequency = config.get('sampling_frequency')
else:
    print("PYTHON: Failed to load configuration.")

circle_centers = load_coordinates("coords.bin")
plot_circles(circle_centers, r, L)
plot_points_in_box(circle_centers, r, L)
p_fractions = load_p_fractions("p_fractions.bin")
plot_p_fractions(p_fractions, sampling_frequency)
