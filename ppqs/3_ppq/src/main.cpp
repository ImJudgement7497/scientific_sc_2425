#include <iostream>
#include <iomanip>
#include <vector>
#include "vtimer_t.h"
#include <cmath>
#include <omp.h>

using namespace std;

// int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
// void debugging_4_threads(int my_thread)
// {

//     if (my_thread == 0)
//     {
//         sum1 += 1;
//     }
//     else if (my_thread == 1)
//     {
//         sum2 += 1;
//     }
//     else if (my_thread == 2)
//     {
//         sum3 += 1;
//     }
//     else
//     {
//         sum4 += 1;
//     }

//     printf("%d, %d, %d, %d, %d \n", sum1, sum2, sum3, sum4);
// }
void serial()
{

    vector<int> N_values = {
        1000, 2000, 5000,
        10000, 20000, 50000,
        100000, 200000, 500000,
        1000000, 2000000, 5000000};

    vtimer_t timer;
    for (int j = 0; j < N_values.size(); ++j)
    {
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0);
        timer.start();
        double sum = 0;

        for (int i = 0; i < N; ++i)
        {
            A[i] = i + 1;
            B[i] = A[i] * A[i];
            A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;
            sum += A[i];
        }
        timer.stop();
        string message = "N value: " + to_string(N) + " Sum: " + to_string(sum) + " Time: " + to_string(timer.elapsed_time());
        printf("%s \n", message.c_str());
    }
}

void parallel()
{
    // vector<int> N_values = {
    //     1000, 2000, 5000,
    //     10000, 20000, 50000,
    //     100000, 200000, 500000,
    //     1000000, 2000000, 5000000};

    vector<int> N_values = {5000000};

    for (int j = 0; j < N_values.size(); ++j)
    {
        int my_thread;
        int total_threads;
        int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0);
        double sum = 0;
#pragma omp parallel default(none) shared(sum1, sum2, sum3, sum4, N, A, B) private(my_thread, total_threads) reduction(task, + : sum)
        {
#pragma omp single
            for (int i = 0; i < N; ++i)
#pragma omp task firstprivate(i) in_reduction(+ : sum)
            {
                A[i] = i + 1;
                B[i] = A[i] * A[i];
                A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;
                sum += A[i];

                // my_thread = omp_get_thread_num();
                // total_threads = omp_get_num_threads();

                // if (my_thread == 0)
                // {
                //     sum1 += 1;
                // }
                // else if (my_thread == 1)
                // {
                //     sum2 += 1;
                // }
                // else if (my_thread == 2)
                // {
                //     sum3 += 1;
                // }
                // else
                // {
                //     sum4 += 1;
                // }
            }
            // printf("%d, %d, %d, %d \n", sum1, sum2, sum3, sum4);
        }

        string message = "N value: " + to_string(N) + " Sum: " + to_string(sum) + " Time: ";
        printf("%s \n", message.c_str());
    }
}

int main()
{
    // cout << "------------SERIAL----------------" << endl;
    // serial();
    cout << "------------PARALLEL--------------" << endl;
    parallel();
    return 0;
}