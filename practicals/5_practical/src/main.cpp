#include <iostream>
#include <vector>
#include <mpi.h>
#include <stdio.h>
#include <cmath>

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


double get_energy(int &myrank, int &size, double(&r0)[2])
{

    int ierror;
    double local_energy = 0, local_distance = 0;
    MPI_Status status;
    MPI_Request sen_request;
    MPI_Request rec_request;

    double left[2], right[2];

    int my_right = myrank + 1;
    int my_left = myrank - 1;

    if (my_right >= size)
    {
        my_right = 0;
    }

    if (my_left < 0)
    {
        my_left = size - 1;
    }

    // Send to the right, recieve from the left
    ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, my_right, 17, MPI_COMM_WORLD, &sen_request);
    ierror = MPI_Wait(&sen_request, &status);
    ierror = MPI_Irecv(&left[0], 2, MPI_DOUBLE, my_left, 17, MPI_COMM_WORLD, &rec_request);
    ierror = MPI_Wait(&rec_request, &status);

    local_distance = pow((r0[1] - left[1]), 2) + pow((r0[0] - left[0]), 2);
    local_energy += 0.5 * local_distance;

    // Send to the left, recieve from the right
    ierror = MPI_Isend(&r0[0], 2, MPI_DOUBLE, my_left, 17, MPI_COMM_WORLD, &sen_request);
    ierror = MPI_Wait(&sen_request, &status);
    ierror = MPI_Irecv(&right[0], 2, MPI_DOUBLE, my_right, 17, MPI_COMM_WORLD, &rec_request);
    ierror = MPI_Wait(&rec_request, &status);

    local_distance = pow((r0[1] - right[1]), 2) + pow((r0[0] - right[0]), 2);
    local_energy += 0.5 * local_distance;

    return local_energy / 2;
}

int main(int argc, char **argv)
{

    MPI_Init (&argc, &argv);

    int myrank, size;
    double local_energy, energy;
    // Basic processor info
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    // Initalise position array
    // MPI_Send(&mydata, 1, MPI_INT, 1, 178, MPI_COMM_WORLD);
    double r0[2];
    if (myrank == 0)
    {
        r0[0] = 0.0;
        r0[1] = 0.0;
        local_energy = get_energy(myrank, size, r0);    
    }

    if (myrank == 1)
    {   
        r0[0] = 1.0;
        r0[1] = 1.0;
        local_energy = get_energy(myrank, size, r0);
    }

    if (myrank == 2)
    {
        r0[0] = 0.5;
        r0[1] = 0.5;
        local_energy = get_energy(myrank, size, r0);
    }

    if (myrank == 3)
    {
        r0[0] = 0.2;
        r0[1] = 0.7;
        local_energy = get_energy(myrank, size, r0);
    }

    int ierror = MPI_Reduce(&local_energy, &energy, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Finalize ();
    cout << energy << endl;
    exit (0);
}
