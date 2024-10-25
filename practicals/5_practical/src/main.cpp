#include <iostream>
#include <vector>
#include <mpi.h>
#include <stdio.h>

using namespace std;

// Question 1 
// int main(int argc, char **argv)
// {

//     int myrank, size;

//     MPI_Init (&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
//     MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

//     cout << "Hello World from rank " << myrank+1 << " of " << size << endl;

//     MPI_Finalize ();

//     exit (0);
// }

// Question 2
int main(int argc, char **argv)
{

    MPI_Init (&argc, &argv);

    // Basic processor info
    int myrank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    // Initalise position array
    vector<double> r0(2, 0.0);

    if (myrank == 0)
    {
        r0 = {0.0, 0.0};
        cout << myrank << r0[0] << r0[1] << endl;
    }

    if (myrank == 1)
    {   
        r0 = {1.0, 1.0};
        cout << myrank << r0[0] << r0[1] << endl;
    }

    if (myrank == 2)
    {
        r0 = {0.5, 0.5};
        cout << myrank << r0[0] << r0[1] << endl;
    }

    if (myrank == 3)
    {
        r0 = {0.2, 0.7};
        cout << myrank << r0[0] << r0[1] << endl;
    }


    MPI_Finalize ();

    exit (0);
}
