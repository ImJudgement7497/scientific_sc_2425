#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <omp.h>
#include <mutex>
#include "rng.h"
/* NEED TO APPLY FORMATTING TO ALL THE FILES */
using namespace std;

/* ------------------------------GLOBAL VARIABlE------------------------------ */
double L;                        // Length of box (read in from input)
double r;                        // Radius of circle (read in from input)
int sampling_frequency;          // The frequency of trials before checking for convergence (read in from input)
double r_comp;                   // Value to compare against
int grid_size;                   // Grid size (dependent on r)
const size_t NUM_MUTEXES = 1024; // Fixed number of mutexes

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
unordered_map<Cell, vector<Point>, CellHash> trial_grid;

// /* Array of mutex locks */
// array<mutex, NUM_MUTEXES> mutex_grid;
// array<mutex, NUM_MUTEXES> trial_mutex_grid;

// Helper to compute the grid cell for a point - specify inline for the compiler
inline Cell get_grid_cell(const Point &p)
{
    return {static_cast<int>(p.first / (L / grid_size)), static_cast<int>(p.second / (L / grid_size))};
}

// Check if a new circle overlaps with circles in nearby grid cells
int check_overlap(pair<Point, bool> &trial_placement, unordered_map<Cell, vector<Point>, CellHash> &map)
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
            if (map.find(neighbor_cell) != map.end())
            {
                for (const auto &existing_circle : map[neighbor_cell])
                {

                    if (new_circle == existing_circle)
                    {
                        continue; // Skip self comparison
                    }
                    // Compute squared distance for comparison
                    double delta_x = existing_circle.first - new_circle.first;
                    double delta_y = existing_circle.second - new_circle.second;
                    double distance_squared = delta_x * delta_x + delta_y * delta_y;

#ifdef DENUG
                    cout << "Checking overlap between circle at ("
                         << new_circle.first << ", " << new_circle.second << ") "
                         << "and existing circle at (" << existing_circle.first << ", "
                         << existing_circle.second << ") with distance squared = "
                         << distance_squared << endl;

#endif

                    if (distance_squared < r_comp && distance_squared != 0)
                    {
                        trial_placement.second = true; // If overlap, set the flag to true
                        return 0;
                    }
                }
            }
        }
    }

    return 1; // No overlap detected
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

// size_t get_mutex_index(const Cell &cell)
// {
//     return CellHash{}(cell) % NUM_MUTEXES; // Use your custom CellHash
// }

// // void place_circle_in_grid(const Point &circle)
// // {
// //     Cell cell = get_grid_cell(circle);
// //     size_t mutex_index = get_mutex_index(cell);

// //     // Use the associated mutex based on the hashed cell
// //     lock_guard<mutex> lock(mutex_grid[mutex_index]);
// //     grid[cell].push_back(circle);
// // }

// // void place_circle_in_trial_grid(const Point &circle)
// // {
// //     Cell cell = get_grid_cell(circle);
// //     size_t mutex_index = get_mutex_index(cell);

// //     // Use the associated mutex based on the hashed cell
// //     lock_guard<mutex> lock(trial_mutex_grid[mutex_index]);
// //     trial_grid[cell].push_back(circle);
// // }

void place_circle(const Point &circle, unordered_map<Cell, vector<Point>, CellHash> &grid)
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

    /* Initalise random number generator */
    rng random_gen;
    random_gen.seed(1829233); // Make this a user parameter

    /* Initalise data types*/
    bool is_overlapping;
    vector<Point> circle_coords;
    vector<double> p_fractions;
    vector<pair<Point, bool>> trial_placements(sampling_frequency);

    // Determine the grid size dynamically based on r
    grid_size = static_cast<int>(L / (2 * r));

    /* Place first circle */
    Point first_circle = gen_random_pair(random_gen);
    circle_coords.push_back(first_circle);
    place_circle(first_circle, grid);

    /* Calculate first packing fraction */

    double P;
    double P_const = M_PI * r * r / (L * L);
    // p_fractions.push_back(P);

    size_t previous_size = 0;
    int sample_interval = sampling_frequency / 4; // MAKE THIS A USER PARAMETER

    bool done = false;
    double start_time = omp_get_wtime();

#pragma omp parallel shared(done, trial_placements, circle_coords, grid, trial_grid)
    {
        rng local_random_gen;
        int tid = omp_get_thread_num();
        local_random_gen.seed(tid * 1829233 + omp_get_num_threads()); // Unique seed per thread

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
#pragma omp critical
                {
#ifdef DEBUG

                    cout << "Thread " << tid << " placing circle (" << new_circle.first << ", " << new_circle.second << ")" << endl;
#endif // DEBUG
                    place_circle(new_circle, trial_grid);
                }
            }

// Sync all threads, ensuring that trials are generated before checking overlaps
#pragma omp barrier

// **Overlap Check Between Trials Internally**
// All trials generated by all threads must be compared with each other.
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue; // Check shared flag to stop early
                check_overlap(trial_placements[i], trial_grid);
            }
#ifdef DEBUG

#pragma omp single
            {
                for (int i = 0; i < sampling_frequency; i++)
                {
                    cout << i << " " << trial_placements[i].second << endl;
                }
            }
#endif // DEBUG

// Sync all threads after overlap check
#pragma omp barrier

// **Overlap Check Between Trials and Grid**
#pragma omp for schedule(dynamic)
            for (int i = 0; i < sampling_frequency; i++)
            {
                if (done)
                    continue; // Check shared flag to stop work early
                check_overlap(trial_placements[i], grid);

                // if (i != 0 && i % sample_interval == 0) // Every "sampling_frequency / 4" intervals
                // {
                //     current_size = circle_coords.size();
                //     P = current_size * P_const;
                //     p_fractions.push_back(P);
                // }
            }

#pragma omp barrier

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
                        place_circle(new_circle, grid);      // Place circle in the grid
                    }
                }
            }
#pragma omp barrier

// Only one thread performs the convergence check
#pragma omp single
            {
                size_t current_size = circle_coords.size();
#ifdef VIS

                cout << "SIZE OF CIRCLE " << current_size << endl;
                cout << "------------------------" << endl;
#endif // VIS
                if (current_size == previous_size)
                {
                    // If no progress, we can stop the simulation (convergence reached)
                    P = current_size * P_const;
                    p_fractions.push_back(P); // Store packing fraction
                    done = true;              // Signal threads to stop
                }
                else
                {
                    previous_size = current_size;
                    trial_placements.clear();
                    trial_grid.clear();
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
    write_vector(p_fractions, "p_fractions.bin");
    write_string_to_file(message, "./data.txt");
    return 0;
}
