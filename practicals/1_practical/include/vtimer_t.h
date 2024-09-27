#ifndef VTIMER_T_HPP
#define VTIMER_T_HPP

#include <chrono>

//----------------------------------------------------------
// simple class for performing code timing
//----------------------------------------------------------
class vtimer_t
{
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;

public:
    // Start the timer
    void start();

    // Stop the timer
    void stop();

    // Get the elapsed time in seconds
    double elapsed_time();
};

#endif // VTIMER_T_HPP
