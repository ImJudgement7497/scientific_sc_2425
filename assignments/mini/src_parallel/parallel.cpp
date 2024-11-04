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
void get_local_indices(int size, vector<int> &starting_indices, vector<int> &ending_indices)
{
    starting_indices.resize(size);
    ending_indices.resize(size);

    for (int i = 0; i < size; i++)
    {
        starting_indices[i] = i * GRID_SIZE * (GRID_SIZE - 2) / size;
        ending_indices[i] = (GRID_SIZE * GRID_SIZE - 1) - ((GRID_SIZE * (GRID_SIZE - 2) * (size - i - 1)) / (size));

        // cout << "RANK " << i << ": Start: " << starting_indices[i] << ": End: " << ending_indices[i] << endl;
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

vector<int> get_iteration_indices(vector<int> &inner_indices)
{

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
void fill_sources(vector<double> &grid)
{
    grid[get_index(5.0, 5.0)] = 10.0;
    grid[get_index(4.0, 6.0)] = 7.2;
    grid[get_index(7.0, 2.5)] = -1.2;
}

/* Get the location of heat sources on local grids*/
void get_local_sources(vector<double> &local_grid, vector<int> &local_source_indices, vector<double> &local_source_values)
{
    for (int i = 0; i < local_grid.size(); i++)
    {
        if (local_grid[i] != 0.0)
        {
            local_source_indices.push_back(i);
            local_source_values.push_back(local_grid[i]);
        }
    }
}

/* Fill the local grids with source information */
void fill_local_sources(vector<double> &local_grid, vector<int> &local_source_indices, vector<double> &local_source_values)
{
    for (int i = 0; i < local_source_indices.size(); i++)
    {
        int index = local_source_indices[i];
        local_grid[index] = local_source_values[i];
    }
}

vector<double> scatter_inital_data(int rank, int size, vector<double> &full_grid, vector<int> &iterating_indices)
{
    // Get the indices to split the work out
    vector<int> starting_indices, ending_indices, full_iterating_indices;
    get_local_indices(size, starting_indices, ending_indices);
    get_iteration_indices(full_iterating_indices);

    // if (rank == 0)
    // {
    //     print_vector(starting_indices);
    //     print_vector(ending_indices);
    // }

    int start_index = starting_indices[rank];
    int end_index = ending_indices[rank];

    for (int i = 0; i < full_iterating_indices.size(); i++)
    {
        int value = full_iterating_indices[i];

        if (value >= start_index + GRID_SIZE && value <= end_index - GRID_SIZE)
        {
            iterating_indices.push_back(value);
        }
    }

    MPI_Bcast(iterating_indices.data(), iterating_indices.size(), MPI_INT, 0, MPI_COMM_WORLD);
    // {
    //     cout << "Rank " << rank << endl;
    //     print_vector(iterating_indices);
    // }
    // How many elements are each processor reciving and the starting index of each send
    int counts[size];
    int displacment[size];

    for (int i = 0; i < size; i++)
    {
        counts[i] = ending_indices[i] - starting_indices[i] + 1;
        displacment[i] = starting_indices[i];
    }

    // for (int i = 0; i < size; i++)
    // {
    //     cout << counts[i] << endl;
    // }

    // Define a local grid by the number of local indices
    vector<double> local_grid(counts[rank]);

    MPI_Scatterv(full_grid.data(), counts, displacment, MPI_DOUBLE,
                 local_grid.data(), counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return local_grid;
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
    fill_sources(new_grid); // The heat sources do not change across each step

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
    // check_processor_initalisation(rank, size);

    // Full grid without any splitting (all data will eventually end up back on this grid)
    vector<double> full_grid;
    vector<int> iterating_indices;

    // Rank 0 processor initalise the full grid with sources, ready to send
    if (rank == 0)
    {
        full_grid.resize(GRID_SIZE * GRID_SIZE);
        /* TESTING */
        for (int i = 0; i < full_grid.size(); i++)
        {
            full_grid[i] = i;
        }
        // UNCOMMENT BELOW
        //  fill_sources(full_grid);
    }

    vector<double> local_grid = scatter_inital_data(rank, size, full_grid, iterating_indices);
    // {

    //     cout << "Rank: " << rank << endl;
    //     print_vector(local_grid);
    // }
    // Need a local vector for where and what the sources are
    vector<int> local_source_indices;
    vector<double> local_source_values;
    get_local_sources(local_grid, local_source_indices, local_source_values);
    fill_local_sources(local_grid, local_source_indices, local_source_values);
    // {
    //     cout << "Rank " << rank << endl;
    //     print_vector(local_grid);
    // }
    // Initalise two grids, one to be used for current iteration, one for next iteration
    vector<double> current_grid(GRID_SIZE * GRID_SIZE, 0.0);
    vector<double> next_grid(GRID_SIZE * GRID_SIZE, 0.0);

    vector<int> inner_indices;

    bool convergence = false;
    int iterations = 0;

    vtimer_t timer;
    timer.start();

    // Perform the first step
    fill_sources(current_grid);
    next_grid = step(current_grid, inner_indices);

    // Value considered
    int index = get_index(5.5, 5.5);

    while (!convergence)
    {
        if (allclose(next_grid, current_grid, TOLERANCE))
        {
            timer.stop();
            // printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", next_grid[index],
            //        iterations, TOLERANCE, timer.elapsed_time());
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

    return 0;
}