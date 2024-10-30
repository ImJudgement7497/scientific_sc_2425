#include "parallel.h" // Include the header file for access to global variables and functions
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int result = execute_parallel();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        if (result == 0)
        {
            cout << "Simulation completed successfully." << endl;
        }
        else
        {
            cout << "An error occurred during simulation." << endl;
        }
    }

    // Finalize MPI once at the end
    MPI_Finalize();

    return 0;
}