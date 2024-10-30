#include "parallel.h" // Include the header file for access to global variables and functions

int main(int argc, char **argv)
{
    // Call the execute_serial function
    int result = execute_parallel(argc, argv);

    // Check the result of the execution
    if (result == 0)
    {
        cout << "Simulation completed successfully." << endl;
    }
    else
    {
        cout << "An error occurred during simulation." << endl;
    }

    return 0; // Exit the program
}