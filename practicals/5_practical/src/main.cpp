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
    int myrank, size, local_energy, ierror;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    // Initalise position array
    double r0[2], left[2], right[2];
    // MPI_Send(&mydata, 1, MPI_INT, 1, 178, MPI_COMM_WORLD);
    if (myrank == 0)
    {

        MPI_Request sen_request;
        MPI_Request rec_request;
        r0[0] = 0.0;
        r0[1] = 0.0;

        // Send to the right, recieve from the left
        ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD, &sen_request);
        ierror = MPI_Wait(&sen_request, &status);
        ierror = MPI_Irecv(&left[0], 2, MPI_DOUBLE, 3, 17, MPI_COMM_WORLD, &rec_request);
        ierror = MPI_Wait(&rec_request, &status);

        cout << "Rank " << myrank << ": " << left[0] << " " << left[1] << endl;

    }

    if (myrank == 1)
    {   
        MPI_Request sen_request;
        MPI_Request rec_request;
        r0[0] = 1.0;
        r0[1] = 1.0;
        ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, 2, 17, MPI_COMM_WORLD, &sen_request);
        ierror = MPI_Wait(&sen_request, &status);
        ierror = MPI_Irecv(&left[0], 2, MPI_DOUBLE, 0, 17, MPI_COMM_WORLD, &rec_request);
        ierror = MPI_Wait(&rec_request, &status);

        cout << "Rank " << myrank << ": " << left[0] << " " << left[1] << endl;
    }

    if (myrank == 2)
    {
        MPI_Request sen_request;
        MPI_Request rec_request;
        r0[0] = 0.5;
        r0[1] = 0.5;
        ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, 3, 17, MPI_COMM_WORLD, &sen_request);
        ierror = MPI_Wait(&sen_request, &status);
        ierror = MPI_Irecv(&left[0], 2, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD, &rec_request);
        ierror = MPI_Wait(&rec_request, &status);

        cout << "Rank " << myrank << ": " << left[0] << " " << left[1] << endl;
    }

    if (myrank == 3)
    {
        MPI_Request sen_request;
        MPI_Request rec_request;
        r0[0] = 0.2;
        r0[1] = 0.7;
        ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, 0, 17, MPI_COMM_WORLD, &sen_request);
        ierror = MPI_Wait(&sen_request, &status);
        ierror = MPI_Irecv(&left[0], 2, MPI_DOUBLE, 2, 17, MPI_COMM_WORLD, &rec_request);
        ierror = MPI_Wait(&rec_request, &status);

        cout << "Rank " << myrank << ": " << left[0] << " " << left[1] << endl;
    }


    MPI_Finalize ();

    exit (0);
}
