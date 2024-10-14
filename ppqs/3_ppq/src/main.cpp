#include <iostream>
#include <iomanip>
#include <vector>
#include "vtimer_t.h"
#include <cmath>
#include <omp.h>
#include <numeric>

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
        double sum = 0.0;
        double compensation = 0.0;

        for (int i = 0; i < N; ++i)
        {
            A[i] = i + 1;
            B[i] = A[i] * A[i];
            A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;
            double y = A[i] - compensation;
            double t = sum + y;
            compensation = (t - sum) - y;
            sum = t;
        }
        timer.stop();
        string message = "N value: " + to_string(N) + " Sum: " + to_string(sum) + " Time: " + to_string(timer.elapsed_time());
        printf("%s \n", message.c_str());
    }
}

double KahanSum(const vector<double> &input)
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

void parallel()
{
    vector<int> N_values = {
        1000, 2000, 5000,
        10000, 20000, 50000,
        100000, 200000, 500000,
        1000000, 2000000, 5000000};

    for (int j = 0; j < N_values.size(); ++j)
    {
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0);
        double start_time = omp_get_wtime();

        vector<double> thread_sums(omp_get_max_threads(), 0.0);
        vector<double> thread_compensations(omp_get_max_threads(), 0.0);

#pragma omp parallel default(none) shared(A, B, N, thread_sums, thread_compensations)
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

        double total_sum = KahanSum(thread_sums);
        double total_compensation = KahanSum(thread_compensations);

        total_sum += total_compensation;

        double end_time = omp_get_wtime();

        printf("N value: %d Sum: %f Time: %f\n", N, total_sum, end_time - start_time);
    }
}

int main()
{
    cout << "------------SERIAL----------------" << endl;
    serial();
    cout << "------------PARALLEL--------------" << endl;
    parallel();
    return 0;
}