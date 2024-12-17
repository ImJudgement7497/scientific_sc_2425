#include "rng.h"

// Seed the RNG with a uniform distribution [0:1)
void rng::seed(unsigned int random_seed)
{
    dist = std::uniform_real_distribution<double>(0.0, 1.0);
    std::mt19937::result_type mt_seed = random_seed;
    mt.seed(mt_seed); // Seed generator
}

// Generate a uniform random number between 0 and 1
double rng::grnd()
{
    return dist(mt);
}