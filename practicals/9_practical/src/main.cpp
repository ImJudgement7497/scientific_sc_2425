#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void write_vector_binary(const string file_name, vector<int> &vec)
{
    ofstream ofile(file_name, ios::binary);
    ofile.write(reinterpret_cast<const char *>(&vec[0]), sizeof(int) * vec.size());
}



int main(int argc, char **argv)
{

    vector<int> local_vec;
    int rank, size;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get size

    // mpi file handle and status variables
    MPI_File fh;
    MPI_Status status;
    // initialise buffer with rank number
    const int N = 100;
    const int num_data = N/size;

    std::vector<int> buffer(num_data, rank);
    vector<int>read_buffer(num_data, rank);
    // // open file collectively (all processors in communicator)
    // MPI_File_open(MPI_COMM_WORLD, "test.txt", MPI_MODE_CREATE|MPI_MODE_RDWR,
    // MPI_INFO_NULL, &fh);

    // MPI_Offset offset = rank * (N/size) * sizeof(int);
    // // write buffer to shared file only on rank 0
    // if(rank == 0)
    // {
    // MPI_File_write(fh, &buffer[0], buffer.size(), MPI_INT, &status);
    // }
    // // collectively close file (all processors in communicator)
    // MPI_File_close(&fh);
    // MPI_File_write_at(fh, offset, &buffer[0], buffer.size(), MPI_INT, &status);
    
    // if (rank==0)
    // {
    //     cout << "Finished writing" << endl;
    // }

    // MPI_File_read_at(fh, offset, &read_buffer[0], read_buffer.size(), MPI_INT, &status);


    stringstream file_name_ss;
    file_name_ss << rank << "_vec_binary.out";
    string file_name = file_name_ss.str();
    write_vector_binary(file_name, buffer);

    read_buffer = read_vector_file(file_name);

    for (int i = 0; i < size; i++)
    {
        if (rank == i)
        {   
            cout << "Rank " << i << endl;
            for (int j = 0; j < read_buffer.size(); j++)
            {
                cout << read_buffer[j] << ", ";
            }
            cout << endl;
        }
    }
    
    MPI_Finalize ();

    exit (0);
}