#include "parallel.h" // Include the header file for access to global variables and functions

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    // Call the execute_parallel function
    execute_parallel();

    // // Check the result of the execution
    // if (result == 0)
    // {
    //     cout << "Simulation completed successfully." << endl;
    // }
    // else
    // {
    //     cout << "An error occurred during simulation." << endl;
    // }

    MPI_Finalize();

    return 0; // Exit the program
}