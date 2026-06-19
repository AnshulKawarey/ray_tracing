#ifndef RTINCLUDE_H
#define RTINCLUDE_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

// Helper Constants and Functions
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    // Return a random real number in [0,1)
    // return std::rand() / (RAND_MAX + 1.0);
    // Each thread gets its own gen and distribution
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline double random_double(double min, double max){
    // Return a random real number in [0,1)
    return min + (max-min) * random_double();
}

#endif