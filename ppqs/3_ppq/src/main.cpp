#include <iostream>
#include <iomanip>
#include <vector>
#include "vtimer_t.h"
#include <cmath>
#include <omp.h>

using namespace std;

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

    vector<int> N_values = {
        1000, 2000, 5000,
        10000, 20000, 50000,
        100000, 200000, 500000,
        1000000, 2000000, 5000000};

    int my_thread = omp_get_thread_num();

    for (int j = 0; j < N_values.size(); ++j)
    {
        int N = N_values[j];
        vector<double> A(N, 0.0), B(N, 0.0);
        double start_time = omp_get_wtime();
        double sum = 0;
#pragma omp parallel for default(none) shared(A, B, N) reduction(+ : sum)
        for (int i = 0; i < N; ++i)
        {
            A[i] = i + 1;
            B[i] = A[i] * A[i];
            A[i] = A[i] + sqrt(abs(sin(B[i]))) * 2.34;
            sum += A[i];
        }
        double end_time = omp_get_wtime();

        string message = "N value: " + to_string(N) + " Sum: " + to_string(sum) + " Time: " + to_string(end_time - start_time);
        printf("%s \n", message.c_str());
    }
}

int main()
{
    // serial();
    parallel();
    return 0;
}