#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <omp.h>
#include "rng.h"
/* NEED TO APPLY FORMATTING TO ALL THE FILES */
using namespace std;

/* ------------------------------GLOBAL VARIABlE------------------------------ */
double L;               // Length of box (read in from input)
double r;               // Radius of circle (read in from input)
double r_comp;          // Value to compare against
int seed;               // Seed for random number generator (read in from input)
int sampling_frequency; // The frequency of trials before checking for convergence
int grid_size;          // Grid size (dependent on r)
int num_of_threads;     // Number of threads

typedef pair<double, double> Point; // Pre-define type for ease
typedef pair<int, int> Cell;        // Pre-define type for ease

/* ------------------------------GRID UTILITIES------------------------------ */

/* Cell Hash needed for the unordered map */
/* It is strange that C++ does not have a hash for standard data structures */
struct CellHash
{
    size_t operator()(const pair<int, int> &cell) const
    {
        size_t h1 = hash<int>{}(cell.first);
        size_t h2 = hash<int>{}(cell.second);
        return h1 ^ (h2 << 1);
    }
};

/* Map of grid cells to points in the grid */
unordered_map<Cell, vector<Point>, CellHash> grid;

// Helper to compute the grid cell for a point - specify inline for the compiler
inline Cell get_grid_cell(const Point &p)
{
    return {static_cast<int>(p.first / (L / grid_size)), static_cast<int>(p.second / (L / grid_size))};
}

// Check if a new circle overlaps with circles in nearby grid cells
int check_overlap(pair<Point, bool> &trial_placement)
{

    Point new_circle = trial_placement.first;

    Cell cell = get_grid_cell(new_circle);

    // Check nearby neighbour cells
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            Cell neighbor_cell = {cell.first + dx, cell.second + dy};

            // Check if neighbor cell exists in the spatial grid
            if (grid.find(neighbor_cell) != grid.end())
            {
                for (const auto &existing_circle : grid[neighbor_cell])
                {
                    // Compute squared distance for comparison
                    double dx = existing_circle.first - new_circle.first;
                    double dy = existing_circle.second - new_circle.second;
                    double distance_squared = dx * dx + dy * dy;

                    if (distance_squared < r_comp)
                    {
                        trial_placement.second = true; // If overlap, return true
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

/* ------------------------------I/O FUNCTIONS------------------------------ */

/* Writes a vector of pairs to a binary file */
void write_coordinates(const vector<Point> &coordinates, const string &file_name)
{
    ofstream file(file_name, ios::binary);
    if (file.is_open())
    {
        // Write each coordinate (pair of doubles) to the file
        for (const auto &coord : coordinates)
        {
            file.write(reinterpret_cast<const char *>(&coord.first), sizeof(coord.first));
            file.write(reinterpret_cast<const char *>(&coord.second), sizeof(coord.second));
        }
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << file_name << endl;
    }
}

/* Writes a vector of doubles to a binary file */
void write_vector(const vector<double> &vec, const string &file_name)
{
    ofstream file(file_name, ios::binary);
    if (file.is_open())
    {
        // Write each double to the file
        for (const auto &elm : vec)
        {
            file.write(reinterpret_cast<const char *>(&elm), sizeof(elm));
        }
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << file_name << endl;
    }
}

/* Writes a string to a file */
void write_string_to_file(const string &data, const string &file_name)
{
    ofstream file(file_name, ios::app);
    if (file.is_open())
    {
        file << data << "\n";
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << file_name << endl;
    }
}

/* Parses and loads the config file */
/* CAN MAKE THIS BETTER BY NOT HAVING THE IF STATEMENTS, BUT WORK FOR NOW */
bool load_config(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        // Skip empty lines or lines that are comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t pos = line.find(' ');
        if (pos != string::npos)
        {
            string key = line.substr(0, pos);        // Key is the part before the space
            string value_str = line.substr(pos + 1); // Value is the part after the space

            float value = stof(value_str);

            // Assign the value to the corresponding parameter
            if (key == "L")
            {
                L = value;
            }
            else if (key == "r")
            {
                r = value;
            }
            else if (key == "sampling_frequency")
            {
                sampling_frequency = value;
            }
            else
            {
                cerr << "Unknown parameter: " << key << endl;
                return false;
            }
        }
    }

    r_comp = (2 * r) * (2 * r); // Pre-compute comparison
    file.close();
    return true;
}

/* ------------------------------SIMULATION FUNCTIONS------------------------------*/

/* Updates grid with circle */
void place_circle(const Point &circle)
{
    Cell cell = get_grid_cell(circle);
    grid[cell].push_back(circle);
}

/* Generates and returns a Point, within the boundaries */
Point gen_random_pair(rng &random_gen)
{
    return {
        r + (L - 2 * r) * random_gen.grnd(),
        r + (L - 2 * r) * random_gen.grnd(),
    };
}

int main()
{
    // Load configuration
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }

    cout << "Running with L = " << L << ", r = " << r << ", and sf = " << sampling_frequency << endl;

#pragma omp parallel shared(num_of_threads)
    {
#pragma omp single
        {
            num_of_threads = omp_get_num_threads();
        }
    }

    /* Initalise random number generator */
    rng random_gen;
    random_gen.seed(seed);

    /* Initalise data types*/
    bool is_overlapping;
    vector<Point> circle_coords;
    vector<pair<Point, bool>> trial_placements(sampling_frequency);

    // Determine the grid size dynamically based on r
    grid_size = static_cast<int>(L / (2 * r));

    /* Place first circle */
    Point first_circle = gen_random_pair(random_gen);
    circle_coords.push_back(first_circle);
    place_circle(first_circle);

    double P;
    double P_const = M_PI * r * r / (L * L);

    size_t previous_size = 0;
    int sample_interval = sampling_frequency / 4; // MAKE THIS A USER PARAMETER

    double start_time = omp_get_wtime();

    bool done = false;

#pragma omp parallel shared(done, trial_placements, circle_coords, grid, seed)
    {
        rng local_random_gen;
        int tid = omp_get_thread_num();
        local_random_gen.seed(tid * seed + omp_get_num_threads()); // Unique seed per thread

        while (!done)
        {
// Parallel loop to generate trials
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue; // Check shared flag to stop early

                Point new_circle = gen_random_pair(local_random_gen); // Generate new trial circle
                trial_placements[i] = {new_circle, false};            // Store it globally
            }

// Sync all threads, ensuring that trials are generated before checking overlaps
#pragma omp barrier

            // **Overlap Check Between Trials and Grid**
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue; // Check shared flag to stop work early

                check_overlap(trial_placements[i]);
            }

// Sync all threads after overlap check
#pragma omp barrier

// **Overlap Check Between Trials Internally**
// All trials generated by all threads must be compared with each other.
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue; // Check shared flag to stop early

                if (trial_placements[i].second)
                    continue; // Skip raised flag already

                for (int j = i + 1; j < sampling_frequency; j++) // Compare each trial with others
                {
                    // Compare trial[i] with trial[j] for overlap
                    Point circle1 = trial_placements[i].first;
                    Point circle2 = trial_placements[j].first;

                    double dx = circle2.first - circle1.first;
                    double dy = circle2.second - circle1.second;
                    double distance_squared = dx * dx + dy * dy;

                    if (distance_squared < r_comp)
                    {
                        // Set overlap flag for one trial if they overlap, the other is still possible trial
                        trial_placements[i].second = true;
                        break;
                    }
                }
            }

#pragma omp barrier

// #pragma omp single
//             {
//                 for (int i = 0; i < sampling_frequency; i++)
//                 {
//                     cout << trial_placements[i].second << endl;
//                 }
//             }

// Place the trials in the grid based on their flag
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue;

                // cout << "Checking trial " << i << ": " << trial_placements[i].second << " (overlap status)\n";

                if (!trial_placements[i].second) // No overlap
                {
                    // cout << "Placing trial " << i << " into grid\n";

// Enter critical section to update shared resources
#pragma omp critical
                    {
                        Point new_circle = trial_placements[i].first;
                        circle_coords.push_back(new_circle); // Add to circle coordinates
                        place_circle(new_circle);            // Place circle in the grid
                    }
                }
            }

#pragma omp barrier

// Only one thread performs the convergence check
#pragma omp single
            {
                size_t current_size = circle_coords.size();
#ifdef VIS

                printf("\rNumber of Circles: %zu", current_size);
                fflush(stdout);
#endif // VIS
                if (current_size == previous_size)
                {
                    // If no progress, we can stop the simulation (convergence reached)
                    P = current_size * P_const;
                    done = true; // Signal threads to stop
                }
                else
                {
                    previous_size = current_size;
                    trial_placements.clear();
                }
            }
        }
    }
    double end_time = omp_get_wtime();
    double elpased_time = end_time - start_time;
    size_t current_size = circle_coords.size();

    string message = "Number of circles: " + to_string(current_size) + ", Packing Fraction = " + to_string(P) + ", Time = " + to_string(elpased_time);
    cout << endl;
    cout << message << endl;

    write_coordinates(circle_coords, "coords.bin");
    write_string_to_file(message, "./data.txt");
    write_string_to_file(to_string(current_size), "./num_of_circles.txt");
    write_string_to_file(to_string(elpased_time), "./times.txt");
    return 0;
}
