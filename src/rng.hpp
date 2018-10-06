#ifndef RNG_HEADER
#define RNG_HEADER

#include <iostream>
#include <random>
#include <chrono>

// Mersennes' twister
static std::mt19937_64 rg((std::chrono::high_resolution_clock::now().time_since_epoch().count()));

// Generate random doubles in the interval [initial, last]
double random_double(double initial, double last) {
    std::uniform_real_distribution<double> distribution(initial, last);
    return distribution(rg);
}


// Generate random int in the interval [initial, last]
int random_integer(int initial, int last) {
    std::uniform_int_distribution<int> distribution(initial, last);
    return distribution(rg);
}

#endif
