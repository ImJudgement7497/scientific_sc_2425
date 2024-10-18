#include <iostream>
#include <vector>
#include <mpi.h>
#include <stdio.h>

using namespace std;

// Question 1 
int main(int argc, char **argv)
{

    int myrank, size;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    cout << "Hello World from rank " << myrank+1 << " of " << size << endl;

    MPI_Finalize ();

    exit (0);
}