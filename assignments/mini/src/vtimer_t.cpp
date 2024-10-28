#include "vtimer_t.h"

// Start the timer
void vtimer_t::start()
{
    start_time = std::chrono::high_resolution_clock::now();
}

// Stop the timer
void vtimer_t::stop()
{
    end_time = std::chrono::high_resolution_clock::now();
}

// Get the elapsed time in seconds
double vtimer_t::elapsed_time()
{
    // Work out elapsed time
    return 1.e-9 * double(
                       std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}
