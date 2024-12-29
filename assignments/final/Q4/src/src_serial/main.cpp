#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include "vtimer_t.h"
#include "rng.h"
/* NEED TO APPLY FORMATTING TO ALL THE FILES */
using namespace std;

/* ------------------------------GLOBAL VARIABlE------------------------------ */
double L;               // Length of box (read in from input)
double r;               // Radius of circle (read in from input)
double r_comp;          // Value to compare against
int sampling_frequency; // The frequency of trials before checking for convergence
int grid_size;          // Grid size (dependent on r)

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
bool check_overlap(const Point &new_circle)
{
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
                        return true; // If overlap, return true
                    }
                }
            }
        }
    }
    return false; // No overlap found
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
    /* Load config file*/
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }

#ifdef DEBUG
    cout << "DEBUGGING ENABLED" << endl;
#endif

    cout << "Running with L = " << L << " and r = " << r << " and sf = " << sampling_frequency << endl;

    /* Initalise random number generator */
    rng random_gen;
    random_gen.seed(1829233); // Make this a user parameter

    vtimer_t timer;
    timer.start();

    /* Initalise data types*/
    bool is_overlapping;
    vector<Point> circle_coords;
    vector<double> p_fractions;

    // Determine the grid size dynamically based on r
    grid_size = static_cast<int>(L / (2 * r));

    /* Place first circle */
    Point first_circle = gen_random_pair(random_gen);
    circle_coords.push_back(first_circle);
    place_circle(first_circle);

    /* Calculate first packing fraction */
    size_t current_size = circle_coords.size();
    double P;
    double P_const = M_PI * r * r / (L * L);
    // p_fractions.push_back(P);

    size_t previous_size = 0;
    int k = 0;
    int sample_interval = sampling_frequency / 4; // MAKE THIS A USER PARAMETER

    while (true)
    {
        for (int i = 0; i < sampling_frequency; i++)
        {
#ifdef VIS
            if (k < p_fractions.size())
            {
                printf("\r k = %d, i = %d, size = %zu, P = %f", k, i, circle_coords.size(), p_fractions[k]);
            }
            else
            {
                printf("\r k = %d, i = %d, size = %zu", k, i, circle_coords.size());
            }
            fflush(stdout);
#endif
            Point new_circle = gen_random_pair(random_gen);

            // Check for overlaps in the grid
            if (!check_overlap(new_circle))
            {
                circle_coords.push_back(new_circle);
                place_circle(new_circle);
            }

            if (i != 0 && i % sample_interval == 0) // Every "sampling_frequency / 4" intervals
            {
                current_size = circle_coords.size();
                P = current_size * P_const;
                p_fractions.push_back(P);
            }
        }

        // Check for convergence
        current_size = circle_coords.size();
        if (current_size == previous_size)
        {
            // Add final packing fraction
            P = current_size * P_const;
            p_fractions.push_back(P);
            break;
        }
        else
        {
            previous_size = current_size;
            k++;
        }
    }

    timer.stop();
    // Get the last packing fraction
    P = p_fractions.back();

    // Outputs all necessary data
    string message = "Number of circles: " + to_string(current_size) + ", Packing Fraction = " + to_string(P) + ", Time = " + to_string(timer.elapsed_time());
    cout << endl;
    cout << message << endl;

    write_coordinates(circle_coords, "coords.bin");
    write_vector(p_fractions, "p_fractions.bin");
    write_string_to_file(message, "./results/serial_results/runs_serial.txt");
    write_string_to_file(message, "./data.txt");

    return 0;
}