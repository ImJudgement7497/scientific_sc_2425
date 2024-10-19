#include "vtimer_t.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <vector>
#include <fstream>

using namespace std;

// Apply Kahan Summation algorithm for a vector of values
double kahan_sum(const vector<double> &input)
{
    double sum = 0.0;
    double c = 0.0;

    for (double num : input)
    {
        double y = num - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

// Perform algorithm and output the results without any parallisation
void serial(vector<int> &N_values)
{

    vtimer_t timer; // Start timer
    for (int j = 0; j < N_values.size(); ++j)
    {
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0); // Allocate vectors
        timer.start();
        double sum = 0.0;
        double compensation = 0.0;

        for (int i = 0; i < N; ++i)
        {
            A[i] = i + 1; // Perform the algorithm
            B[i] = A[i] * A[i];
            A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;

            /*
             Instead of doing sum += A[i] we apply the Kahan summation algorithm
             for more accurate sums when N is very large
            */
            double y = A[i] - compensation;
            double t = sum + y;
            compensation = (t - sum) - y;
            sum = t;
        }
        timer.stop(); // End timer
        string message = "N value: " + to_string(N) +
                         " Sum: " + to_string(sum) +
                         " Time: " + to_string(timer.elapsed_time());
        printf("%s \n", message.c_str());
    }
}

// Perform algorithm and output results with parallisation
void parallel(vector<int> &N_values)
{

    for (int j = 0; j < N_values.size(); ++j)
    {
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0); // Allocate vectors A and B
        double start_time = omp_get_wtime(); // Start parallel timer

        /* Allocate vectors for each thread sum*/
        vector<double> thread_sums(omp_get_max_threads(), 0.0);
        vector<double> thread_compensations(omp_get_max_threads(), 0.0);

// Go parallel
#pragma omp parallel default(none) \
    shared(A, B, N, thread_sums, thread_compensations)
        {
            // Get the thread number, and initalise a local sum and compensation for KS algorithm
            int tid = omp_get_thread_num();
            double local_sum = 0.0;
            double local_compensation = 0.0;

#pragma omp for
            for (int i = 0; i < N; ++i)
            {
                // Apply algorithm
                A[i] = i + 1;
                B[i] = A[i] * A[i];
                A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;

                /*
                Instead of doing sum += A[i] we apply the Kahan summation algorithm
                for more accurate sums when N is very large
                */
                double y = A[i] - local_compensation;
                double t = local_sum + y;
                local_compensation = (t - local_sum) - y;
                local_sum = t;
            }

            // No race conditions as each thread has a unique id
            thread_sums[tid] = local_sum;
            thread_compensations[tid] = local_compensation;
        }

        // Instead of reduction, kahan sum the array
        double total_sum = kahan_sum(thread_sums);

        double end_time = omp_get_wtime();

        printf("N value: %d Sum: %f Time: %f\n", N, total_sum,
               end_time - start_time);
    }
}

// Similar functiom to above, but for only one N value
double parallel_for_mean(int &N)
{

    vector<double> A(N, 0.0), B(N, 0.0);
    double start_time = omp_get_wtime();

    vector<double> thread_sums(omp_get_max_threads(), 0.0);
    vector<double> thread_compensations(omp_get_max_threads(), 0.0);

#pragma omp parallel default(none) \
    shared(A, B, N, thread_sums, thread_compensations)
    {
        int tid = omp_get_thread_num();
        double local_sum = 0.0;
        double local_compensation = 0.0;

#pragma omp for
        for (int i = 0; i < N; ++i)
        {
            A[i] = i + 1;
            B[i] = A[i] * A[i];
            A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;

            double y = A[i] - local_compensation;
            double t = local_sum + y;
            local_compensation = (t - local_sum) - y;
            local_sum = t;
        }

        thread_sums[tid] = local_sum;
        thread_compensations[tid] = local_compensation;
    }

    double total_sum = kahan_sum(thread_sums);

    double end_time = omp_get_wtime();

    return end_time - start_time;
}

// Function to run the parallel algorithm and average the time
void analysis()
{
    int N = 1000000;
    vector<double> times;
    double time_sum = 0.0;

    for (int i = 0; i < 100; i++)
    {
        times.push_back(parallel_for_mean(N));
    }

    for (double num : times)
    {

        time_sum += num;
    }

    ofstream file;
    string file_path = "./results/parra_times_VIKING.txt";
    file.open(file_path, ios::app);

    file << (time_sum / times.size()) << endl;
    file.close();
}

/* This main is for getting results for all N values*/
int main()
{
    vector<int> N_values = {1000, 2000, 5000, 10000, 20000, 50000,
                            100000, 200000, 500000, 1000000, 2000000, 5000000};
    cout << "RUNNING ON " << omp_get_max_threads() << " THREADS" << endl;
    cout << "------------SERIAL----------------" << endl;
    serial(N_values);
    cout << "------------PARALLEL--------------" << endl;
    parallel(N_values);
    return 0;
}

/* This main is for analysis*/
// int main()
// {
//     analysis();
// }