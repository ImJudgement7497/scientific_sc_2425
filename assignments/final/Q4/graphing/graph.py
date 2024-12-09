import matplotlib.pyplot as plt

def load_coordinates(filename):
    with open(filename, "r") as file:
        coordinates = []
        for line in file:
            x, y = map(float, line.split())
            coordinates.append((x, y))
    return coordinates

circle_centers = load_coordinates("coords.txt")
radius = 1.234

def plot_circles(centers, radius):
    fig, ax = plt.subplots()
    
    for (x, y) in centers:
        # Create a circle at (x, y) with the given radius
        circle = plt.Circle((x, y), radius, color='blue', fill=False, linewidth=2)
        ax.add_patch(circle)
        # Optionally plot the center
        ax.plot(x, y, 'ro')  # Red dot for the center
    
    # Set the aspect ratio to equal so circles appear correctly
    ax.set_aspect('equal', adjustable='box')
    ax.set_xlim(min(x for x, y in centers) - radius - 1, max(x for x, y in centers) + radius + 1)
    ax.set_ylim(min(y for x, y in centers) - radius - 1, max(y for x, y in centers) + radius + 1)
    plt.grid(True)
    plt.title("Circles")
    plt.savefig("Plot")

# Call the function to plot
plot_circles(circle_centers, radius)
