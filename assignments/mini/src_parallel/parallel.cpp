#include "parallel.h"
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

/*-------------------------------CONFIG FUNCTIONS-------------------------------*/

/* Get the indices of the grid that each processor needs to work on*/
void get_sub_indices(int size)
{
    for (int i = 0; i < size; i++)
    {
    int start_index = i*GRID_SIZE*(GRID_SIZE-2) / size;
    int end_index = (GRID_SIZE*GRID_SIZE-1) - ((GRID_SIZE*(GRID_SIZE-2)*(size - i - 1)) / (size)); 

    cout << "RANK " << i << ": Start: " << start_index << ": End: " << end_index << endl;

    }
}

/* Check if all processors are initalised*/
void check_processor_initalisation(int rank, int size)
{
    cout << "Processor " << rank << " of " << size << " initalised." << endl;
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

/*-------------------------------INDICE FUNCTIONS-------------------------------*/

/* Map the (x, y) coordinate to a singular index to be used in a 1D array */
int get_index(double x, double y)
{
    int col_index = round(x / GRID_STEP);
    int row_index = round(y / GRID_STEP);
    int index = row_index * GRID_SIZE + col_index;

    return index;
}

vector<int> get_inner_indices()
{
    vector<int> inner_indices;

    for (int row_number = 1; row_number < GRID_SIZE - 1; row_number++)
    {
        int starting_index = row_number * GRID_SIZE + 1;
        int ending_index = (row_number + 1) * GRID_SIZE - 2;

        for (int i = starting_index; i <= ending_index; i++)
        {
            inner_indices.push_back(i);
        }
    }

    return inner_indices;
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

/* ----------------------------------SIMULATION FUNCTIONS--------------------------------------------------*/

/* Initialise the heat sources */
void fill_heat_sources(vector<double> &grid)
{
    grid[get_index(5.0, 5.0)] = 10.0;
    grid[get_index(4.0, 6.0)] = 7.2;
    grid[get_index(7.0, 2.5)] = -1.2;
}

/* A step in time for the simulation */
vector<double> step(vector<double> &current_grid, const vector<int> &inner_indices)
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
    for (int i = 0; i < inner_indices.size(); i++)
    {
        // pair<double, double> coord = get_coordinates(index);

        // // Boundaries on X
        // if (coord.first == GRID_MIN || coord.first == GRID_MAX)
        // {
        //     continue;
        // }
        int index = inner_indices[i];
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
        INNER_GRID_SIZE = sqrt(j);
    }
    return new_grid;
}

/* Execute the simulation*/
int execute_parallel()
{
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size
    check_processor_initalisation(rank, size);

    if (rank == 0)
    {
        get_sub_indices(size);
    }
    // Initalise two grids, one to be used for current iteration, one for next iteration
    vector<double> current_grid(GRID_SIZE * GRID_SIZE, 0.0);
    vector<double> next_grid(GRID_SIZE * GRID_SIZE, 0.0);

    vector<int> inner_indices = get_inner_indices();

    bool convergence = false;
    int iterations = 0;

    vtimer_t timer;
    timer.start();

    // Perform the first step
    fill_heat_sources(current_grid);
    next_grid = step(current_grid, inner_indices);

    // Value considered
    int index = get_index(5.5, 5.5);

    while (!convergence)
    {
        if (allclose(next_grid, current_grid, TOLERANCE))
        {
            timer.stop();
            printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", next_grid[index],
                   iterations, TOLERANCE, timer.elapsed_time());
            convergence = true;
        }
        else
        {
            /* Below shows the user the values as they are iterated*/
            // // Print the current value
            // printf("\rValue = %.16f", current_grid[index]);
            // fflush(stdout); // Ensure it flushes to the terminal

            // printf("\r%s", string(30, ' ').c_str()); // Clear the line (30 spaces)

            // Update grids for next iteration
            current_grid = next_grid;
            next_grid = step(current_grid, inner_indices);
        }
        iterations++;
    }

    log_global_variables();
    generate_mappings();
    log_sources();

    MPI_Finalize();

    return 0;
}