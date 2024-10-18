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

    int myrank, size, mydata;
    MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    mydata = myrank;

    if (myrank == 0)
    {
        int ierr = MPI_Send(&mydata, 1, MPI_INT, 1, 178, MPI_COMM_WORLD);
        cout << "Rank 0 sent: " << mydata << endl;
        int ierr2 = MPI_Recv(&mydata, 1, MPI_INT, 1, 178, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Rank 0 recieved: " << mydata << endl;
    }

    if (myrank == 1)
    {   
    
        int ierr3 = MPI_Recv(&mydata, 1, MPI_INT, 0, 178, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Rank 1 recieved: " << mydata << endl;
        mydata = myrank;
        int ierr4 = MPI_Send(&mydata, 1, MPI_INT, 0, 178, MPI_COMM_WORLD);
        cout << "Rank 1 sent: " << mydata << endl;
    }


    MPI_Finalize ();

    exit (0);
}
