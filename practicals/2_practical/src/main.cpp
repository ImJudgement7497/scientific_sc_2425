#include <iostream>
#include <vector>
#include <omp.h>
#include <string>
#include <vtimer_t.h>

using namespace std;

void question_1() {

    int my_thread;
    int total_threads;

    #pragma omp parallel private(my_thread, total_threads)
    {
        my_thread = omp_get_thread_num();
        total_threads = omp_get_num_threads();
        string message = "Hello world from thread number " + to_string(my_thread) + " of " + to_string(total_threads);
    
        cout << message << endl;
    }
}

long question_2(const long N) {

    long sum = 0;

    for (int i = 1; i<=N; ++i) {

        sum += i;
    }

    return sum;
}

pair<long, double> question_3(const long N) {

    long sum = 0;
    double total_time = 0;
    int i;
    double start = 0;
    double end = 0;
    pair<long, double> pair;

    #pragma omp parallel private(i, start, end) reduction(+:total_time)
    {
        start = omp_get_wtime();
        #pragma omp for private(i, start, end) reduction(+:sum)
            for (i = 1; i<=N; ++i) {
                sum += i;
            }
        end = omp_get_wtime();
        total_time = end - start;
    }   

    pair.first = sum;
    pair.second = total_time;

    
    return pair;


}

int main()
{
    vtimer_t timer;

    timer.start();
    long N = 1000000;

    cout << "Serial: " << question_2(N) << endl;
    timer.stop();
    cout << "Serial Time: " << timer.elapsed_time() << endl;

    pair<long, double> parra = question_3(N);
    cout << "Parallel: " << parra.first << endl;
    cout << "Parallel Time: " << parra.second << endl;
    return 0;
}