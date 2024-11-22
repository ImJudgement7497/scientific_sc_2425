#include <iostream>
#include <vector>
#include <mpi.h>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{

    int rank, size;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    // cout << "Hello World from rank " << myrank+1 << " of " << size << endl;

    int num_of_ranks = 32;
    int num_of_nodes = 2;
    int num_of_sockets_per_node = 2;
    int num_of_cores_per_socket = 8;
    
    int num_of_cores_per_node = num_of_cores_per_socket * num_of_sockets_per_node;

    if (rank==0)
    {
        for (int i = 0; i < num_of_ranks; i++)
        {
            int node_id = i / num_of_cores_per_node;
            int socket_id = (i - (node_id * num_of_cores_per_node)) / num_of_cores_per_socket;
            int core_id = i % num_of_cores_per_socket;
            printf("global id: %d   node id: %d socket id: %d   core id: %d thread id: %d \n",
                i, node_id, socket_id, core_id, 0);

        }
    }





    MPI_Finalize ();
    return 0;

    
}