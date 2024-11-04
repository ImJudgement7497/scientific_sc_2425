#include "serial.h"
#include "helper.h"
#include "vtimer_t.h"
#include <thread>
#include <chrono>

/*-------------------------------GLOBAL VARIABLES*-------------------------------*/
int GRID_SIZE; // For an N point grid, you need N+1 grid size
double GRID_MIN;
double GRID_MAX;
double GRID_STEP;
int INNER_GRID_SIZE = 0; // Assigned within
int INNER_GRID_MIN_INDEX;
int INNER_GRID_MAX_INDEX;
double TOLERANCE;

/*-------------------------------SIMULATION FUNCTIONS-------------------------------*/

double safe_access(const vector<double> &grid, int index)
{
    try
    {
        return grid.at(index);
    }
    catch (const out_of_range &)
    {
        return 0.0;
    }
}

/* Write to a log all the global variables */
void log_global_variables()
{
    ofstream log_file("./logs/global.log", ios::trunc);

    if (!log_file)
    {
        cerr << "Error: Could not open log file at ./logs/global.log" << endl;
        return;
    }

    log_file << "GRID_SIZE = " << GRID_SIZE << endl;
    log_file << "GRID_MIN = " << GRID_MIN << endl;
    log_file << "GRID_MAX = " << GRID_MAX << endl;
    log_file << "GRID_STEP = " << GRID_STEP << endl;
    log_file << "INNER_GRID_MIN_INDEX = " << INNER_GRID_MIN_INDEX << endl;
    log_file << "INNER_GRID_MAX_INDEX = " << INNER_GRID_MAX_INDEX << endl;
    log_file << "TOLERANCE = " << TOLERANCE << endl;
    log_file << "INNER_GRID_SIZE = " << INNER_GRID_SIZE << endl;

    log_file.close();
}

void generate_mappings()
{
    string filename = "./mappings/mappings_" + to_string(GRID_SIZE) + ".txt";

    ofstream file(filename, ios::trunc);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    for (double i = GRID_MIN; i <= GRID_MAX; i += GRID_STEP)
    {
        file << "X Y Index" << endl;
        file << "---------------------------------------------------" << endl;
        for (double j = GRID_MIN; j <= GRID_MAX; j += GRID_STEP)
        {
            file << j << " " << i << " :" << get_index(j, i) << endl;
        }
    }

    file.close();
}

void log_sources()
{
    string filename = "./logs/sources.log";

    ofstream file(filename, ios::trunc);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    file << "FOR GRID_SIZE = " << GRID_SIZE << ", GRID_STEP = " << GRID_STEP << endl;
    file << "Index for source T=10 at (5.0, 5.0) = " << get_index(5.0, 5.0) << endl;
    file << "Index for source T=7.2 at (4.0, 6.0) = " << get_index(4.0, 6.0) << endl;
    file << "Index for source T=-1.2 at (7.0, 2.5) = " << get_index(7.0, 2.5) << endl;

    file.close();
}

/* Load a configuration file */
bool load_config(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    file >> GRID_SIZE >> GRID_MIN >> GRID_MAX >> TOLERANCE;

    file.close();

    GRID_STEP = (GRID_MAX - GRID_MIN) / (GRID_SIZE - 1);

    // Boundaries on Y
    INNER_GRID_MIN_INDEX = 1 + GRID_SIZE;
    INNER_GRID_MAX_INDEX = ((GRID_SIZE * GRID_SIZE) - 1 - GRID_SIZE);

    return true;
}

/* Map the (x, y) coordinate to a singular index to be used in a 1D array */
int get_index(double x, double y)
{
    int col_index = round(x / GRID_STEP);
    int row_index = round(y / GRID_STEP);
    int index = row_index * GRID_SIZE + col_index;

    return index;
}

/* Given an index, get the (x, y) coordinate */
pair<double, double> get_coordinates(int index)
{
    int row_index = index / GRID_SIZE; // Calculate row
    int col_index = index % GRID_SIZE; // Calculate column

    double x = col_index * GRID_STEP; // Convert column to x coordinate
    double y = row_index * GRID_STEP; // Convert row to y coordinate

    return {x, y}; // Return as a pair of doubles
}

/* ----------------------------------MAIN FUNCTIONS--------------------------------------------------*/
void fill_outer_grid(vector<double> &outer_grid, vector<double> &inner_grid)
{
    for (int inner_index = 0; inner_index < inner_grid.size(); inner_index++)
    {
        // What row are we on
        int row = inner_index / (GRID_SIZE - 2);

        // What is the corresponding outer index?
        int outer_index = inner_index + GRID_SIZE + 2 * row + 1;

        // Set the values

        outer_grid[outer_index] = inner_grid[inner_index];

        // cout << "Inner Index = " << inner_index << ", Outer Index = " << outer_index << endl;
    }
}
/* Initialise the heat sources */
void fill_sources(vector<double> &inner_grid)
{
    // What are the outer indices?
    int outer_index_1 = get_index(5.0, 5.0);
    int outer_index_2 = get_index(4.0, 6.0);
    int outer_index_3 = get_index(7.0, 2.5);

    // What are the rows in the grid
    int row_1 = outer_index_1 / GRID_SIZE;
    int row_2 = outer_index_2 / GRID_SIZE;
    int row_3 = outer_index_3 / GRID_SIZE;

    // What are the inner indices?
    int inner_index_1 = outer_index_1 - GRID_SIZE - 1 - 2 * row_1;
    int inner_index_2 = outer_index_2 - GRID_SIZE - 1 - 2 * row_2;
    int inner_index_3 = outer_index_3 - GRID_SIZE - 1 - 2 * row_3;

    inner_grid[inner_index_1] = 10.0;
    inner_grid[inner_index_2] = 7.2;
    inner_grid[inner_index_3] = -1.2;
}

/* A step in time for the simulation */
vector<double> step(vector<double> &current_inner_grid)
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */
    int j = 0;
    vector<double> new_inner_grid((GRID_SIZE - 2) * (GRID_SIZE - 2), 0.0);
    new_inner_grid = current_inner_grid;

    // Note we only iterate through a smaller grid defined by GRID_SIZE - 2 as edge cells stay at T = 0
    for (int index = 0; index < (GRID_SIZE - 2) * (GRID_SIZE - 2); index++)
    {
        // Get neighbouring values
        double current = safe_access(new_inner_grid, index);
        double left = safe_access(new_inner_grid, index - 1);
        double right = safe_access(new_inner_grid, index + 1);
        double up = safe_access(new_inner_grid, index + GRID_SIZE);
        double down = safe_access(new_inner_grid, index - GRID_SIZE);

        new_inner_grid[index] = (current + left + right + up + down) / 5.0;
        j++;
    }
    fill_sources(new_inner_grid); // The heat sources do not change across each step

    if (INNER_GRID_SIZE == 0)
    {
        INNER_GRID_SIZE = j;
    }
    return new_inner_grid;
}

/* Execute the simulation*/
int execute_serial()
{
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }

    vector<double> outer_grid(GRID_SIZE * GRID_SIZE, 0.0);
    // Initalise two grids, one to be used for current iteration, one for next iteration
    vector<double> current_grid((GRID_SIZE - 2) * (GRID_SIZE - 2), 0.0);
    vector<double> next_grid((GRID_SIZE - 2) * (GRID_SIZE - 2), 0.0);

    bool convergence = false;
    int iterations = 0;

    vtimer_t timer;
    timer.start();

    // Perform the first step
    fill_sources(current_grid);
    next_grid = step(current_grid);

    // Value considered
    int index = get_index(5.5, 5.5);

    while (!convergence)
    {
        if (allclose(next_grid, current_grid, TOLERANCE))
        {
            timer.stop();
            fill_outer_grid(outer_grid, next_grid);
            printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", outer_grid[index],
                   iterations, TOLERANCE, timer.elapsed_time());
            convergence = true;
        }
        else
        {
            // Print the current value
            printf("\rValue = %.16f", current_grid[index]);
            fflush(stdout); // Ensure it flushes to the terminal

            printf("\r%s", string(30, ' ').c_str()); // Clear the line (30 spaces)

            // Update grids for next iteration
            current_grid = next_grid;
            next_grid = step(current_grid);
        }
        iterations++;
    }

    log_global_variables();
    generate_mappings();
    log_sources();

    return 0;
}
