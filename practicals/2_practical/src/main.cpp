#include <iostream>
#include <vector>
#include <omp.h>
#include <string>
#include <vtimer_t.h>
#include <cstdio>

using namespace std;

// void question_1()
// {

//     int my_thread;
//     int total_threads;
//     int sum = 0;
//     int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

// #pragma omp parallel for schedule(static, 1) default(none) shared(sum1, sum2, sum3, sum4) private(my_thread, total_threads) reduction(+ : sum)
//     for (int i = 1; i <= 100; i++)
//     {
//         sum += i;
//         my_thread = omp_get_thread_num();
//         total_threads = omp_get_num_threads();

//         if (my_thread == 0)
//         {
//             sum1 += 1;
//         }
//         else if (my_thread == 1)
//         {
//             sum2 += 1;
//         }
//         else if (my_thread == 2)
//         {
//             sum3 += 1;
//         }
//         else
//         {
//             sum4 += 1;
//         }
//         string message = "Hello world from thread number " + to_string(my_thread) + " of " + to_string(total_threads);

//         // std::cout << i << " " << message << std::endl;
//         printf("%d %s \n", i, message.c_str());
//     }
//     printf("%d, %d, %d, %d, %d", sum, sum1, sum2, sum3, sum4);
// }

void question_1()
{

    int my_thread;
    int total_threads;
    int sum = 0;
    int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

#pragma omp parallel default(none) shared(sum1, sum2, sum3, sum4) private(my_thread, total_threads) reduction(task, + : sum)
    {

#pragma omp single
        for (int i = 1; i <= 100; i++)
        {
#pragma omp task firstprivate(i) in_reduction(+ : sum)
            {
                sum += i;
                my_thread = omp_get_thread_num();
                total_threads = omp_get_num_threads();

                if (my_thread == 0)
                {
                    sum1 += 1;
                }
                else if (my_thread == 1)
                {
                    sum2 += 1;
                }
                else if (my_thread == 2)
                {
                    sum3 += 1;
                }
                else
                {
                    sum4 += 1;
                }
                string message = "Hello world from thread number " + to_string(my_thread) + " of " + to_string(total_threads);

                // std::cout << i << " " << message << std::endl;
                printf("%d %s \n", i, message.c_str());
            }
        }

        printf("%d, %d, %d, %d, %d \n", sum, sum1, sum2, sum3, sum4);
    }
}

// long question_2(const long N) {

//     long sum = 0;

//     for (int i = 1; i<=N; ++i) {

//         sum += i;
//     }

//     return sum;
// }

// pair<long, double> question_3(const long N) {

//     long sum = 0;
//     double total_time = 0;
//     int i;
//     double start = 0;
//     double end = 0;
//     pair<long, double> pair;

//     #pragma omp parallel private(i, start, end) reduction(+:total_time)
//     {
//         start = omp_get_wtime();
//         #pragma omp for private(i, start, end) reduction(+:sum)
//             for (i = 1; i<=N; ++i) {
//                 sum += i;
//             }
//         end = omp_get_wtime();
//         total_time = end - start;
//     }

//     pair.first = sum;
//     pair.second = total_time;

//     return pair;

// }

int main()
{
    question_1();
    // vtimer_t timer;

    // timer.start();
    // long N = 1000000;

    // cout << "Serial: " << question_2(N) << endl;
    // timer.stop();
    // cout << "Serial Time: " << timer.elapsed_time() << endl;

    // pair<long, double> parra = question_3(N);
    // cout << "Parallel: " << parra.first << endl;
    // cout << "Parallel Time: " << parra.second << endl;
    return 0;
}