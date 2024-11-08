#include <iostream>
#include <chrono>
#include <random>
#include <vector>
using namespace std;
//----------------------------------------------------------
// simple class for performing code timing
//----------------------------------------------------------
class vtimer_t{

private:
   std::chrono::high_resolution_clock::time_point start_time;
   std::chrono::high_resolution_clock::time_point end_time;

public:
   // start the timer
   void start(){
      start_time = std::chrono::high_resolution_clock::now();
   }

   // start the timer
   void stop(){
      end_time = std::chrono::high_resolution_clock::now();
   }

   // get the elapsed time in milliseconds
   double elapsed_time(){

      // work out elapsed time
      return 1.e-9*double(std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());

   }
};

// void do_saxpy(int N, float A[], float B[], const float x, const float y)
void do_saxpy(int N, vector<float> &A, vector<float> &B, const float x, const float y)
{
    for (int i = 0; i < N; i++)
    {
        A[i] = B[i]*x + y;
    }
}

int main()
{
    int N = 1024;
    int M = 10240000;
    // float A[N];
    // float B[N];
    vector<float> A(N), B(N);
    float x = 0.25f;
    float y = 0.43f;
    vtimer_t timer;

    for (int i = 0; i < N; i++)
    {
        A[i] = i*1.0f;
        B[i] = i*2.0f;
    }

    timer.start();
    for (int j = 0; j < M; j++)
    {
        do_saxpy(N, A, B, x, y);
    }
    timer.stop();

    std::cout << "Time for calculation: " << timer.elapsed_time() <<
   " s (" << double(M)*2.0*double(N)*1.0e-9/timer.elapsed_time() <<
   " GFlops)" << "\t" << A[0] << std::endl;


    
    return 0;

}