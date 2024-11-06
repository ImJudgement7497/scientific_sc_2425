#include "parallel.h"
#include "helper.h"
#include "vtimer_t.h"
#include <thread>
#include <chrono>
#include <unordered_set>

/*-------------------------------GLOBAL VARIABLES*-------------------------------*/
int GRID_SIZE; // For an N point grid, you need N+1 grid size
double GRID_MIN;
double GRID_MAX;
double GRID_STEP;
int NUM_OF_PROCS; // Assigned within
int INNER_GRID_MIN_INDEX;
int INNER_GRID_MAX_INDEX;
double TOLERANCE;

/*-------------------------------CONFIG FUNCTIONS-------------------------------*/

/* Check if all processors are initalised*/
void check_processor_initalisation(int rank, int size)
{
    cout << "Processor " << rank << " of " << size << " initalised." << endl;
}

/* Write to a log all the global variables */
void log_global_variables()
{
    ofstream log_file("./logs/parallel/global.log", ios::trunc);

    if (!log_file)
    {
        cerr << "Error: Could not open log file at ./logs/parallel/global.log" << endl;
        return;
    }

    log_file << "GRID_SIZE = " << GRID_SIZE << endl;
    log_file << "GRID_MIN = " << GRID_MIN << endl;
    log_file << "GRID_MAX = " << GRID_MAX << endl;
    log_file << "GRID_STEP = " << GRID_STEP << endl;
    log_file << "INNER_GRID_MIN_INDEX = " << INNER_GRID_MIN_INDEX << endl;
    log_file << "INNER_GRID_MAX_INDEX = " << INNER_GRID_MAX_INDEX << endl;
    log_file << "TOLERANCE = " << TOLERANCE << endl;
    log_file << "NUM_OF_PROCS = " << NUM_OF_PROCS << endl;

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
    string filename = "./logs/parallel/sources.log";

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

void log_local_sources(unordered_set<int> &source_indices, const string &file_name)
{
    ofstream file(file_name, ios::trunc);

    for (auto &element : source_indices)
    {
        file << element << " ";
    }
    file << endl;
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

/* Get the indices that are needed to be considered in the iteration */
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

/* Initalise all indice arrays needed */
void initalise_indices(int rank, int size, vector<int> &iterating_indices, int counts[], int displacement[])
{
    // Get the indices to split the work out
    vector<int> starting_indices, ending_indices, full_iterating_indices;

    get_local_indices(size, starting_indices, ending_indices);
    get_iteration_indices(full_iterating_indices);

    // What indices are required to be iterated over?
    int start_index = starting_indices[rank];
    int end_index = ending_indices[rank];

    // Go through the full list of iterating indices and give them to each processor
    for (int i = 0; i < full_iterating_indices.size(); i++)
    {
        int value = full_iterating_indices[i];

        if (value >= start_index + GRID_SIZE && value <= end_index - GRID_SIZE)
        {
            iterating_indices.push_back(value);
        }
    }

    // Broadcast the iterating indices so each rank has the same iterating indices
    /* this works mathematically if size is an integer factor of GRID_SIZE-2*/
    MPI_Bcast(iterating_indices.data(), iterating_indices.size(), MPI_INT, 0, MPI_COMM_WORLD);

    // Get the number of data points in each rank, and the starting index for each rank
    for (int i = 0; i < size; i++)
    {
        counts[i] = ending_indices[i] - starting_indices[i] + 1;
        displacement[i] = starting_indices[i];
    }
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
unordered_set<int> get_local_sources(vector<double> &local_grid)
{
    unordered_set<int> source_indices;
    for (int i = 0; i < local_grid.size(); i++)
    {
        if (local_grid[i] != 0.0)
        {
            source_indices.insert(i);
        }
    }
    return source_indices;
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

/* Scatter the grid to processors using counts and displacement*/
vector<double> scatter_grid(int rank, int size, vector<double> &full_grid, int counts[], int displacement[])
{
    // Define a local grid by the number of local indices
    vector<double> local_grid(counts[rank]);

    MPI_Scatterv(full_grid.data(), counts, displacement, MPI_DOUBLE,
                 local_grid.data(), counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return local_grid;
}

/* Gather the grid to root using counts and displacement */
vector<double> gather_grid(int rank, int size, vector<double> &local_grid, int counts[], int displacement[])
{
    vector<double> full_grid;
    if (rank == 0)
    {
        full_grid.resize(displacement[size - 1] + counts[size - 1]);
    }

    MPI_Gatherv(local_grid.data(), counts[rank], MPI_DOUBLE,
                full_grid.data(), counts, displacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return full_grid;
}

/* A step in time for the simulation */
vector<double> step(int rank, int size, vector<double> &full_grid, unordered_set<int> &source_indices,
                    vector<int> &iterating_indices, int counts[], int displacement[])
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */
    int rows_per_proc = (GRID_SIZE - 2) / size;
    vector<double> new_full_grid(full_grid.size(), 0.0);
    new_full_grid = full_grid;

    vector<double> local_grid = scatter_grid(rank, size, new_full_grid, counts, displacement);
    // cout << "Rank " << rank << endl;
    // print_vector(local_grid);

    int above = rank - 1;
    int below = rank + 1;

    MPI_Status status;
    MPI_Request reqs[4];

    // Send bottom row to below and receive top ghost row from below
    /* LOGGING TO BE REMOVED */
    {
        // ofstream logFile;
        // string filename = "./logs/parallel/rank_" + to_string(rank) + "_communication.log";
        // logFile.open(filename, ios::app);

        // Sending the bottom row to the rank below and receiving top ghost row from below
        if (below < size)
        {
            // // Log the data being sent to `below`
            // logFile << "Rank " << rank << " sending to Rank " << below << ": ";
            // for (int i = 0; i < GRID_SIZE; ++i)
            // {
            //     logFile << local_grid[rows_per_proc * GRID_SIZE + i] << " ";
            // }
            // logFile << "\n";
            // logFile << "Sent from index: " << rows_per_proc * GRID_SIZE << " to " << rows_per_proc * GRID_SIZE + GRID_SIZE - 1;
            // logFile << "\n___________________________\n";
            // logFile.flush();

            // Perform send/receive operation
            MPI_Sendrecv(
                &local_grid[rows_per_proc * GRID_SIZE], GRID_SIZE, MPI_DOUBLE, below, 0,
                &local_grid[(rows_per_proc + 1) * GRID_SIZE], GRID_SIZE, MPI_DOUBLE, below, 1,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // // Log the data received from `below`
            // logFile << "Rank " << rank << " received from Rank " << below << " and placed at index range: ";
            // logFile << (rows_per_proc + 1) * GRID_SIZE << " to " << (rows_per_proc + 1) * GRID_SIZE + GRID_SIZE - 1 << "\n";
            // logFile << "Received data: ";
            // for (int i = 0; i < GRID_SIZE; ++i)
            // {
            //     logFile << local_grid[(rows_per_proc + 1) * GRID_SIZE + i] << " ";
            // }
            // logFile << "\n___________________________\n";
            // logFile.flush();
        }

        // Sending the top row to the rank above and receiving bottom ghost row from above
        if (above >= 0)
        {
            // Log the data being sent to `above`
            // logFile << "Rank " << rank << " sending to Rank " << above << ": ";
            // for (int i = 0; i < GRID_SIZE; ++i)
            // {
            //     logFile << local_grid[GRID_SIZE + i] << " ";
            // }
            // logFile << "\n";
            // logFile << "Sent from index: " << GRID_SIZE << " to " << GRID_SIZE + GRID_SIZE - 1;
            // logFile << "\n___________________________\n";
            // logFile.flush();

            // Perform send/receive operation
            MPI_Sendrecv(
                &local_grid[GRID_SIZE], GRID_SIZE, MPI_DOUBLE, above, 1,
                &local_grid[0], GRID_SIZE, MPI_DOUBLE, above, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Log the data received from `above`
            // logFile << "Rank " << rank << " received from Rank " << above << " and placed at index range: ";
            // logFile << "0 to " << GRID_SIZE - 1 << "\n";
            // logFile << "Received data: ";
            // for (int i = 0; i < GRID_SIZE; ++i)
            // {
            //     logFile << local_grid[i] << " ";
            // }
            // logFile << "\n___________________________\n";
            // logFile.flush();
        }

        // Close the log file after all communication is done
        // logFile.close();
    }

    for (int i = 0; i < iterating_indices.size(); i++)
    {
        int index = iterating_indices[i];
        if (source_indices.find(index) != source_indices.end())
        {
            continue;
        }
        // Get neighbouring values
        double current = local_grid[index];
        double left = local_grid[index - 1];
        double right = local_grid[index + 1];
        double up = local_grid[index + GRID_SIZE];
        double down = local_grid[index - GRID_SIZE];

        local_grid[index] = (current + left + right + up + down) / 5.0;
    }

    string file_name = "./logs/parallel/" + to_string(rank) + "_final_grid.log";
    write_vector(local_grid, file_name);

    new_full_grid = gather_grid(rank, size, local_grid, counts, displacement);

    return new_full_grid;
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
    NUM_OF_PROCS = size;

    // Full grid without any splitting (all data will eventually end up back on this grid)
    vector<double> full_grid, next_full_grid, test_vector;
    vector<int> iterating_indices;
    unordered_set<int> source_indices;

    // Rank 0 processor initalise the full grid with sources, ready to send
    if (rank == 0)
    {
        full_grid.resize(GRID_SIZE * GRID_SIZE, 0.0);
        next_full_grid.resize(GRID_SIZE * GRID_SIZE, 0.0);
        /* TESTING */
        // for (int i = 0; i < full_grid.size(); i++)
        // {
        //     if (i >= 0 && i <= 11)
        //     {
        //     full_grid[i] = 0.0;
        //     }
        //     if (i >= 12 && i <=23)
        //     {
        //         full_grid[i] = 1.0;
        //     }
        //     if (i >= 24 && i <= 35)
        //     {
        //         full_grid[i] = 2.0;
        //     }
        // }

        fill_sources(full_grid);
    }

    // How many elements are each processor reciving and the starting index of each send
    int counts[size];
    int displacement[size];

    initalise_indices(rank, size, iterating_indices, counts, displacement);
    if (rank == 0)
    {
        write_vector(iterating_indices, "./logs/parallel/iterating_indices.log");
    }

    // Need source indices to skip over them when doing the calculation
    test_vector = scatter_grid(rank, size, full_grid, counts, displacement);
    source_indices = get_local_sources(test_vector);
    log_local_sources(source_indices, "./logs/parallel/" + to_string(rank) + "_local_sources.log");
    write_vector(test_vector, "./logs/parallel/" + to_string(rank) + "_inital_grid.log");
    // print_vector(iterating_indices);
    // {

    //     cout << "Rank: " << rank << endl;
    //     print_vector(local_grid);
    // }
    // Need a local vector for where and what the sources are
    // vector<int> local_source_indices;
    // vector<double> local_source_values;
    // get_local_sources(local_grid, local_source_indices, local_source_values);
    // fill_local_sources(local_grid, local_source_indices, local_source_values);
    // {
    //     cout << "Rank " << rank << endl;
    //     print_vector(local_grid);
    // }
    // Initalise two grids, one to be used for current iteration, one for next iteration

    bool convergence = false;
    int iterations = 0;

    // vtimer_t timer;
    // timer.start();

    // // Perform the first step
    for (int i = 0; i < 21290; i++)
    {
        next_full_grid = step(rank, size, full_grid, source_indices, iterating_indices, counts, displacement);
        // fill_local_sources(next_full_grid, local_source_indices, local_source_values);
        // cout << "Rank " << rank << endl;
        // print_vector(next_full_grid);
        // if (rank == 0)
        // {
        //     fill_sources(next_full_grid);
        // }
        full_grid = next_full_grid;
    }

    if (rank == 0)
    {
        write_vector(full_grid, "./logs/parallel/full_grid.log");
        double value = full_grid[get_index(5.5, 5.5)];
        cout << value << endl;
        // cout << "___________________" << endl;
        // cout << full_grid.size() << endl;
        // for (int i = 0; i < size; i++)
        // {
        //     cout << displacement[i] << endl;
        // }
        // cout << iterating_indices[0] << endl;
        // cout << iterating_indices[iterating_indices.size() - 1] << endl;
    }
    // // Value considered
    // int index = get_index(5.5, 5.5);
    // cout << full_grid[index] << endl;
    // cout << index << endl;
    // while (!convergence)
    // {
    //     if (allclose(next_full_grid, full_grid, TOLERANCE))
    //     {
    //         timer.stop();
    //         // printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", next_grid[index],
    //         //        iterations, TOLERANCE, timer.elapsed_time());
    //         convergence = true;
    //     }
    //     else
    //     {
    //         /* Below shows the user the values as they are iterated*/
    //         // // Print the current value
    //         // printf("\rValue = %.16f", current_grid[index]);
    //         // fflush(stdout); // Ensure it flushes to the terminal

    //         // printf("\r%s", string(30, ' ').c_str()); // Clear the line (30 spaces)

    //         // Update grids for next iteration
    //         current_grid = next_grid;
    //         next_grid = step(current_grid, inner_indices);
    //     }
    //     iterations++;
    // }

    log_global_variables();
    generate_mappings();
    log_sources();

    return 0;
}