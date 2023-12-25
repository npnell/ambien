#ifndef UTIL_H
#define UTIL_H

#include <limits>
#include <cmath>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = M_PI;

inline double degree_to_rad(double d) {
    return d * pi / 180.0;
}

#endif