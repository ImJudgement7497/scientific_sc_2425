import matplotlib.pyplot as plt
import math

def load_coordinates(filename):
    with open(filename, "r") as file:
        coordinates = []
        for line in file:
            x, y = map(float, line.split())
            coordinates.append((x, y))
    return coordinates

def check_overlap(centers, radius):
    for i, (x1, y1) in enumerate(centers):
        for j, (x2, y2) in enumerate(centers):
            if i != j:
                dist = math.sqrt((x2 - x1)**2 + (y2 - y1)**2)
                if dist < 2 * radius:
                    print(f"Circles {i} and {j} overlap: Distance = {dist}, Required = {2 * radius}")
                    return True
    print("No overlaps detected!")
    return False

def plot_circles(centers, radius, L):
    fig, ax = plt.subplots()

    # if check_overlap(centers, radius):
    #     print("Overlap detected! Adjusting coordinates may be necessary.")
    #     return

    for (x, y) in centers:
        circle = plt.Circle((x, y), radius, color='blue', fill=True, linewidth=0.2) # Make this a user parametr
        ax.add_patch(circle)
        # ax.plot(x, y, 'ro')

    ax.set_aspect('equal', adjustable='box')
    ax.set_xlim((0, L))
    ax.set_ylim(0, L)

    plt.grid(False)
    plt.title("Circles In Box")
    plt.savefig("./plots/Circles_in_Box.png", dpi=300)

def plot_points_in_box(centers, L):
    box_limit = 0.025 * L
    filtered_points = [(x, y) for (x, y) in centers if 0 <= x <= box_limit and 0 <= y <= box_limit]

    fig, ax = plt.subplots()
    for (x, y) in centers:
        circle = plt.Circle((x, y), radius, color='blue', fill=True, linewidth=0.2) # Make this a user parametr
        ax.add_patch(circle)

    ax.set_aspect('equal', adjustable='box')
    ax.set_xlim(0, box_limit)
    ax.set_ylim(0, box_limit)

    plt.grid(False)
    plt.title("Circles in Smaller Box")
    plt.savefig("./plots/Circles_in_Smaller_Box.png", dpi=300)

circle_centers = load_coordinates("coords.txt")
# READ THESE FROM THE USER
radius = 1.234
L = 500  

plot_circles(circle_centers, radius, L)
plot_points_in_box(circle_centers, L)
