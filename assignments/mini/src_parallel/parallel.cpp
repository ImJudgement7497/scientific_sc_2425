#include "parallel.h"
#include "helper.h"
#include "vtimer_t.h"
#include <mpi.h>

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

/* Write to a log all the global variables */
void log_global_variables()
{
    ofstream logFile("./logs/global.log", ios::trunc);

    if (!logFile)
    {
        cerr << "Error: Could not open log file at ./logs/global.log" << endl;
        return;
    }

    logFile << "GRID_SIZE = " << GRID_SIZE << endl;
    logFile << "GRID_MIN = " << GRID_MIN << endl;
    logFile << "GRID_MAX = " << GRID_MAX << endl;
    logFile << "GRID_STEP = " << GRID_STEP << endl;
    logFile << "INNER_GRID_MIN_INDEX = " << INNER_GRID_MIN_INDEX << endl;
    logFile << "INNER_GRID_MAX_INDEX = " << INNER_GRID_MAX_INDEX << endl;
    logFile << "TOLERANCE = " << TOLERANCE << endl;
    logFile << "INNER_GRID_SIZE = " << INNER_GRID_SIZE << endl;

    logFile.close();
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

/* Initialise the heat sources */
void fill_heat_sources(vector<double> &grid)
{
    grid[get_index(5.0, 5.0)] = 10.0;
    grid[get_index(4.0, 6.0)] = 7.2;
    grid[get_index(7.0, 2.5)] = -1.2;
}

/* A step in time for the simulation */
vector<double> step(vector<double> &current_grid)
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */
    int j = 0;
    vector<double> new_grid(GRID_SIZE * GRID_SIZE, 0.0);
    new_grid = current_grid;

    // Note we only iterate through a smaller grid defined by GRID_SIZE - 2 as edge cells stay at T = 0
    for (int index = INNER_GRID_MIN_INDEX; index < INNER_GRID_MAX_INDEX; index++)
    {
        pair<double, double> coord = get_coordinates(index);

        // Boundaries on X
        if (coord.first == GRID_MIN || coord.first == GRID_MAX)
        {
            continue;
        }
        // Get neighbouring values
        double current = new_grid[index];
        double left = new_grid[index - 1];
        double right = new_grid[index + 1];
        double up = new_grid[index + GRID_SIZE];
        double down = new_grid[index - GRID_SIZE];

        new_grid[index] = (current + left + right + up + down) / 5.0;
        j++;
    }
    fill_heat_sources(new_grid); // The heat sources do not change across each step

    if (INNER_GRID_SIZE == 0)
    {
        INNER_GRID_SIZE = j;
    }
    return new_grid;
}

/* Execute the simulation*/
int execute_parallel(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    if (!load_config("./config/config.txt"))
    {
        MPI_Finalize();
        return -1;
    }

    {
        int myrank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
        MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get size

        cout << "Hello World from rank " << myrank + 1 << " of " << size << endl;
    }
    // Initalise two grids, one to be used for current iteration, one for next iteration
    vector<double> current_grid(GRID_SIZE * GRID_SIZE, 0.0);
    vector<double> next_grid(GRID_SIZE * GRID_SIZE, 0.0);

    bool convergence = false;
    int iterations = 0;

    // Perform the first step
    fill_heat_sources(current_grid);
    next_grid = step(current_grid);

    // Value considered
    int index = get_index(5.5, 5.5);

    while (!convergence)
    {
        if (allclose(next_grid, current_grid, TOLERANCE))
        {
            printf("Value = %.16f after %d iterations, tol = %.16f\n", current_grid[index],
                   iterations, TOLERANCE);
            convergence = true;
        }
        else
        {
            current_grid = next_grid;
            next_grid = step(current_grid);
        }
        iterations++;
    }

    log_global_variables();

    MPI_Finalize();
    return 0;
}
