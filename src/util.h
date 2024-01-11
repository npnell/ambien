#ifndef UTIL_H
#define UTIL_H

#include <limits>
#include <cmath>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = M_PI;

inline double degree_to_rad(double d) {
    return d * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif