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

__global__
void add(int N, float *A, float *B, float *C)
{

    int index = blockIdx.x * blockDim.x + threadId.x
}

int main()
{
    // call kernel hello with 10 blocks, 10 threads per block
    // hello<<<5, 5>>>();
    // ensure all computations are finished before exiting
    
    int N = 1000000
    float *A, *B. *C

    cudaMallocManaged(&A, N*sizeof(float))
    cudaMallocManaged(&B, N*sizeof(float))
    cudaMallocManaged(&C, N*sizeof(float))

    for(int i = 0; i < N; i++) B[i] = 1.0f;
    for(int i = 0; i < N; i++) C[i] = 2.0f;


    
    cudaFree(A)
    cudaFree(B)
    cudaFree(C)
    
    
    cudaDeviceSynchronize();
    // end main program with exit code 0 (all OK)
    return 0;
}