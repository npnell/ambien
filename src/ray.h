#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
private:
    vec3 dir;
    point3 orig;
public:
    ray(const vec3& dir, const point3& orig) : dir(dir), orig(orig) {}

    point3 origin() { return orig; }

    vec3 direction() { return dir; }

    point3 at(double t) {
        return orig + t * dir;
    }
};

#endif