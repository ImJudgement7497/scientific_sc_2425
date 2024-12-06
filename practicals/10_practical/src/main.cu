#include <iostream>
#include <vector>
#include <cuda_runtime.h>

using namespace std;

__global__ // all kernels are preceded by __global__ keyword
void hello()
{
    // determine thread ID within block
    printf("Hello from block %d, thread %d\n", blockIdx.x, threadIdx.x);
    return;
}

int main()
{
    // call kernel hello with 10 blocks, 10 threads per block
    hello<<<10, 10>>>();
    // ensure all computations are finished before exiting
    cudaDeviceSynchronize();
    // end main program with exit code 0 (all OK)
    return 0;
}