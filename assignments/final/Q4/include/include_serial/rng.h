#ifndef RNG_H
#define RNG_H

#include <random>

class rng
{
private:
    // std::random variables (internal to class)
    std::mt19937 mt;                             // Mersenne Twister generator
    std::uniform_real_distribution<double> dist; // Uniform real distribution

public:
    // Seed the RNG with a uniform distribution [0:1)
    void seed(unsigned int random_seed);

    // Generate a uniform random number between 0 and 1
    double grnd();
};

#endif // RNG_H
