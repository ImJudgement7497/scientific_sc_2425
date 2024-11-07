# Mini Assignment

## General points

The assignment was challenging with the given boundary being on the points $x=[0, 10], \ y = [0, 10]$ and not one `GRID_STEP` away. It essentially meant I defined two grids, an `OUTER_GRID` and an `INNER_GRID` where the `OUTER_GRID` was defined by the `GRID_SIZE` and `GRID_STEP`, and the `INNER_GRID` defined by `GRID_SIZE - 2` and the same `GRID_STEP`.

The `INNER_GRID` is where the work was being done, as those are the points that are changing. Thus for equal load balancing, this meant that the `INNER_GRID` had to be 100x100 therefore the `OUTER_GRID` had to be 102x102. I made my code work for any grid paramaters, as can be seen in `./config/config.txt`, and example of which is below:

@import "../config/config.txt"

Instead of working with a 2D array, I thought it would be better and easier to work with a 1D array, and create a mapping between coordinates and indices of this 1D array. This is done through `get_index()`, which is below:

```c++
/* Map the (x, y) coordinate to a singular index to be used in a 1D array */
int get_index(double x, double y)
{
    int col_index = round(x / GRID_STEP);
    int row_index = round(y / GRID_STEP);
    int index = row_index * GRID_SIZE + col_index;

    return index;
}
```

The majority of the work rests on the shoulders of this mapping, and example grid mappings can be found in `./mappings`

## Results

The value of temperature I got with the `GRID_SIZE=102` at $(5.5, 5.5)$ was `Value = 4.1715298749715810 after 34618 iterations, tol = 0.0000000000010000, time = 35.747428` in serial, and in parallel:
```txt
Size = 1: Value = 4.1715298749715810 after 34618 iterations, 
tol = 0.0000000000010000, time = 22.853177
Size = 2: Value = 4.1715298749715810 after 34618 iterations, 
tol = 0.0000000000010000, time = 10.985350
Size = 4: Value = 4.1715298749715810 after 34618 iterations, 
tol = 0.0000000000010000, time = 5.649549
Size = 5: Value = 4.1715298749715810 after 34618 iterations, 
tol = 0.0000000000010000, time = 4.632271
Size = 10: Value = 4.1715298749715810 after 34618 iterations, 
tol = 0.0000000000010000, time = 2.800578
```

Note the parallel runs were ran on VIKING, and serial was ran on my home laptop. 

To get similar results, please use the `./runs/run_serial.sh` and `./runs/run_parallel.sh`, changing the number of processors in `run_parallel.sh`to suit the needs. This will both `make` the binary, and execute it.

As can be seen, the parallel speedup is quite obvious, where doubling the number of processors, roughly halves the time taken. This is expected as the hardware executes the same `main()` on all processors at once, and the local grids become smaller in proportion to the number of processors. A graph would be included if I had time.

# Serial Code

The most important functions will be listed below, but the full source code will be provided. For both `src/src_serial` and `src/src_parallel` there are helper functions found in different source files, but not necessary. Both sources also have a `main.cpp`, but also not necessary here.

```c++
int execute_serial()
{
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }

    // Initalise two grids, one to be used for current iteration, one for next iteration
    vector<double> current_grid(GRID_SIZE * GRID_SIZE, 0.0);
    vector<double> next_grid(GRID_SIZE * GRID_SIZE, 0.0);

    // To avoid copying large arrays, create an array of the arrays
    vector<double> grids[2] = {current_grid, next_grid};
    int current_grid_index = 0;
    int other_grid_index = (current_grid_index + 1) % 2;

    vector<int> inner_indices = get_iteration_indices();
    unordered_set<int> source_indices = get_source_indices();

    bool convergence = false;
    int iterations = 0;

    vtimer_t timer;
    timer.start();

    // Perform the first step
    fill_sources(grids[current_grid_index]);
    grids[other_grid_index] = step(grids[current_grid_index], grids[other_grid_index], inner_indices, source_indices);

    // Value considered
    int index = get_index(5.5, 5.5);

    while (!convergence)
    {   
        // Check for convergence
        if (allclose(grids[other_grid_index], grids[current_grid_index], TOLERANCE))
        {
            timer.stop();
            printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", grids[other_grid_index][index],
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
            // These step avoids copying of arrays
            int c = current_grid_index;
            current_grid_index = other_grid_index;
            other_grid_index = c;

            grids[other_grid_index] = step(grids[current_grid_index], grids[other_grid_index], inner_indices, source_indices);
        }
        iterations++;
    }

    log_global_variables();
    generate_mappings();
    log_sources();

    return 0;
}

/* A step in time for the simulation */
vector<double> step(vector<double> &input_grid, vector<double> &output_grid, const vector<int> &inner_indices, unordered_set<int> &source_indices)
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */
    int j = 0;

    // Note we only iterate through a smaller grid defined by GRID_SIZE - 2 as edge cells stay at T = 0
    for (int i = 0; i < inner_indices.size(); i++)
    {
        int index = inner_indices[i];

        // if (source_indices.find(index) != source_indices.end())
        // {
        //     continue;
        // }
        // Get neighbouring values
        double current = input_grid[index];
        double left = input_grid[index - 1];
        double right = input_grid[index + 1];
        double up = input_grid[index + GRID_SIZE];
        double down = input_grid[index - GRID_SIZE];

        output_grid[index] = (current + left + right + up + down) / 5.0;
        j++;
    }
    fill_sources(output_grid); // The heat sources do not change across each step

    if (INNER_GRID_SIZE == 0)
    {
        INNER_GRID_SIZE = sqrt(j);
    }
    return output_grid;
}
```

The main principle is that only the inner grid is worked on in `step()`, then the full grid after a time step is checked against the full grid at the previous time step for convergence. 

# Parallel

This is where things became a lot more difficult, because I had to work out how to exactly split the grid up, as well as where the sources lay and the indices that need to be worked on. I decided to use `MPI_Scatterv` to split up the 1D array between the processors. This was because every processor needed a similar splitting, that being `rows_per_proc + 1`, where `rows_per_proc = (GRID_SIZE - 2) / size`. This allowed each processor have rows that they would work on, and an extra row to communicate with other processors. 

This would cause each processor to have an equal local grid, that was then worked upon using a similar `step()` function. After all grids converged locally, I used `MPI_Gatherv` to gather the data back to a full grid. This came with a lot of challenges due to overlapping nature of the local grids, but I ironed out the bugs to a successful code. Note the parallel code can be further optimised like the serial code, but I ran out of time.

```c++
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
vector<double> step(int rank, int size, vector<double> &local_grid, unordered_set<int> &source_indices,
                    vector<int> &iterating_indices, int counts[], int displacement[])
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */
    int rows_per_proc = (GRID_SIZE - 2) / size;
    vector<double> new_local_grid(local_grid.size(), 0.0);

    int above = rank - 1;
    int below = rank + 1;

    {

        /* The only rows that need communicating are the boundary rows, where the
        local grids overlap. This overlap is bigger for a lower size of processors, but in general
        the penultimate row gets sent up the chain, recieved on the bottom row,  and the second row
        gets sent down the chain, recieved on the top row.*/
        if (below < size)
        {

            MPI_Sendrecv(
                &local_grid[rows_per_proc * GRID_SIZE], GRID_SIZE, MPI_DOUBLE, below, 0,
                &local_grid[(rows_per_proc + 1) * GRID_SIZE], GRID_SIZE, MPI_DOUBLE, below, 1,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (above >= 0)
        {

            MPI_Sendrecv(
                &local_grid[GRID_SIZE], GRID_SIZE, MPI_DOUBLE, above, 1,
                &local_grid[0], GRID_SIZE, MPI_DOUBLE, above, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    for (int i = 0; i < iterating_indices.size(); i++)
    {
        int index = iterating_indices[i];
        if (source_indices.find(index) != source_indices.end())
        {
            new_local_grid[index] = local_grid[index];
            continue;
        }
        // Get neighbouring values
        double current = local_grid[index];
        double left = local_grid[index - 1];
        double right = local_grid[index + 1];
        double up = local_grid[index + GRID_SIZE];
        double down = local_grid[index - GRID_SIZE];

        new_local_grid[index] = (current + left + right + up + down) / 5.0;
    }

    return new_local_grid;
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
    NUM_OF_PROCS = size;

    // Full grid without any splitting (all data will eventually end up back on this grid)
    vector<double> full_grid, next_full_grid, test_vector, new_local_grid;
    vector<int> iterating_indices;
    unordered_set<int> source_indices;

    vtimer_t timer;

    // Rank 0 processor initalise the full grid with sources, ready to send
    if (rank == 0)
    {
        full_grid.resize(GRID_SIZE * GRID_SIZE, 0.0);
        next_full_grid.resize(GRID_SIZE * GRID_SIZE, 0.0);
        fill_sources(full_grid);
    }

    // How many elements are each processor reciving and the starting index of each send
    int counts[size];
    int displacement[size];

    // Get all indice information
    initalise_indices(rank, size, iterating_indices, counts, displacement);
    
    // Need source indices to skip over them when doing the calculation
    /* This is now unecessary */
    test_vector = scatter_grid(rank, size, full_grid, counts, displacement);
    source_indices = get_local_sources(test_vector);

    // Perform the scatter
    vector<double> local_grid = scatter_grid(rank, size, full_grid, counts, displacement);
    int iterations = 0;
    bool all_converged = false;
    if (rank == 0)
    {
        timer.start();
    }
    do
    {
        /* Perform the steps, checking for converge every time. */
        new_local_grid = step(rank, size, local_grid, source_indices, iterating_indices, counts, displacement);
        bool local_convergence = allclose(new_local_grid, local_grid, TOLERANCE, iterating_indices);
        local_grid = new_local_grid;
        /* Reduce all local convergences into one, to check */
        MPI_Allreduce(&local_convergence, &all_converged, 1, MPI_CXX_BOOL, MPI_LAND, MPI_COMM_WORLD);
        iterations++;
    } while (!all_converged);

    full_grid = gather_grid(rank, size, local_grid, counts, displacement);

    if (rank == 0)
    {
        timer.stop();
    }
    int index = get_index(5.5, 5.5);
    if (rank == 0)
    {
        printf("Value = %.16f after %d iterations, tol = %.16f, time = %f\n", full_grid[index],
               iterations, TOLERANCE, timer.elapsed_time());
    }
    log_global_variables();
    generate_mappings();
    log_sources();

    return 0;
}
```

