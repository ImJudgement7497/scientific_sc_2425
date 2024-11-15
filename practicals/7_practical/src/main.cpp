#include <iostream>
#include <vector>
#include "vtimer_t.h"
#include <random>
#include <mpi.h>
#include <omp.h>



using namespace std;

//----------------------------------------------------------
// simple wrapper class for rng
//----------------------------------------------------------
class rng{
// std::random variables (internal to class)
std::mt19937 mt;
// mersenne twister
std::uniform_real_distribution<double> dist;
public:
   // seed rng with uniform distribution [0:1)
   void seed(unsigned int random_seed){
      dist = std::uniform_real_distribution<double>(0.0,1.0);
      std::mt19937::result_type mt_seed = random_seed;
      mt.seed(mt_seed); // seed generator
   }
   // wrapper function generate a uniform random number between 0 and 1
   double grnd(){
      return dist(mt);
   }
};



int main(int argc, char **argv)
{
    int rank, size;
    omp_set_dynamic(false);
    omp_set_num_threads(4);

    // variable to store mpi library threading support
    int mpi_thread_supported = 0;
    // initialise MPI with C bindings and basic thread support
    // Options are:
    // MPI_THREAD_SINGLE Only one thread will execute.
    // MPI_THREAD_FUNNELED If the process is multithreaded, only the thread
    // that called MPI_Init_thread will make MPI calls.
    // MPI_THREAD_SERIALIZED If the process is multithreaded, only one thread
    // will make MPI library calls at one time.
    // MPI_THREAD_MULTIPLE If the process is multithreaded, multiple threads
    // may call MPI at once with no restrictions
    // (Very experimental).
    //
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_thread_supported);
    // check for minimum thread support
    if(mpi_thread_supported < MPI_THREAD_FUNNELED){
    std::cerr << "Error: MPI Library does not support threads" << std::endl;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // #pragma omp parallel
    // {
    //     int tid = omp_get_thread_num();
    //  
    //     printf("Hello World from thread %d of %d on rank %d processor of size %d", tid, num_of_threads, rank, size);

    // }
    int num_threads;

    #pragma omp parallel 
    {
        num_threads = omp_get_num_threads();
    }
    
    vector<rng> random_gens(num_threads);
    double r = 1.0;
    double darts_in_circle = 0.0, darts_total = 0.0;
    double mpi_darts_in_circle = 0.0, mpi_darts_total = 0.0;

    for (int thread = 0; thread < num_threads; thread++)
    {
        random_gens[thread].seed(18293*num_threads + thread*size + rank);
    }

    #pragma omp parallel shared(random_gens, r)
    {   
        int tid = omp_get_thread_num();

        #pragma omp for reduction(+:darts_in_circle) reduction(+:darts_total)
        for (int i = 0; i < 10000000; i++)
        {
            double random_x = 2 * r * random_gens[tid].grnd() - r;
            double random_y = 2 * r * random_gens[tid].grnd() - r;
            
            double distance = sqrt(random_x * random_x + random_y * random_y);

            if (distance <= r)
            {
                darts_in_circle += 1.0;
            }
            
            darts_total += 1.0;
        }
    }

    MPI_Reduce(&darts_in_circle, &mpi_darts_in_circle, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&darts_total, &mpi_darts_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank==0)
    {
        printf("Value of PI = %.16f, after %f iterations \n", 4 * mpi_darts_in_circle / mpi_darts_total, mpi_darts_total);
    }

    MPI_Finalize();

    return 0;
}