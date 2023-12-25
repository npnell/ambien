#ifndef INTERVAL_H
#define INTERVAL_H

#include "util.h"

class interval {
public:
    double min;
    double max;

    interval() : min(+infinity), max(-infinity) {}
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) {
        return min <= x && x <= max;
    }

    bool surrounds(double x) {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif