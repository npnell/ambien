#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray(const vec3& dir, const point3& orig) : dir(dir), orig(orig) {}

    point3 origin() const { return orig; }

    vec3 direction() const { return dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }
private:
    vec3 dir;
    point3 orig;
};

#endif