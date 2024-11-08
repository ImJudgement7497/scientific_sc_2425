// Simple program to test SIMD performance
// (c) R F Evans 2021
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

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

#define ALIGNED16
//__attribute__((aligned(16)))

//----------------------------------------------------------
// program to compute the multiplication of A = B*C
// AVX512 - 512 bits (16)
// AVX2 - 256 bits (8)
// SSE3 - 128 bits (4)
//----------------------------------------------------------
int main(){

   const unsigned int N = 512;
   const unsigned int M = 10240000;

   float A[N] ALIGNED16;
   float B[N] ALIGNED16;
   float C[N] ALIGNED16;

   //std::vector<float> A(N), B(N), C(N);

   // initialise B and C with random numbers
   for(int i = 0; i<N; ++i){
      A[i] = float(i+1)*0.01f;
      B[i] = float(i+1)*0.02f;
   }

   rng mt;
   mt.seed(23456);
   int index = mt.grnd()*(N-1); // 122

   // initialise timer
   vtimer_t timer;

   timer.start();

   for(int j=0; j<M; j++){
      // now compute multiplication
      for(int i = 0; i<N; ++i){
         A[i] = B[i]*3.0f + 0.04f;
      }
      A[index] = A[index]+0.01f;
   }

   // stop the timer
   timer.stop();

   std::cout << "Time for calculation: " << timer.elapsed_time() <<
   " s (" << double(M)*2.0*double(N)*1.0e-9/timer.elapsed_time() <<
   " GFlops)" << "\t" << A[index] << std::endl;

   return 0;

}
