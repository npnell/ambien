#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "vec3.h"
#include "ray.h"

class aabb {
public:
    aabb() = default;
    aabb(const interval& _x, const interval& _y, const interval& _z)
        : x(_x), y(_y), z(_z) {}
    aabb(const point3& a, const point3& b) {
        x = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axis(int n) const {
        if(n == 1)  return y;
        if(n == 2)  return z;
        return x;
    }

    /*
    bool hit(const ray& r, interval ray_t) const {
        for(int i = 0; i < 3; ++i) {
            auto t0 = fmin((axis(i).min - r.origin()[i]) / r.direction()[i],
                           (axis(i).max - r.origin()[i]) / r.direction()[i]);
            auto t1 = fmax((axis(i).min - r.origin()[i]) / r.direction()[i],
                           (axis(i).max - r.origin()[i]) / r.direction()[i]);
            ray_t.min = fmax(t0, ray_t.min);
            ray_t.max = fmin(t1, ray_t.max);

            if(ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
    */

    /*
        * The following is a more optimized version of the above AABB hit method.
        * Note that the performance is compiler dependent.
    */
   
    bool hit(const ray& r, interval ray_t) const {
        for(int i = 0; i < 3; ++i) {
            auto inv_d = 1 / r.direction()[i];
            auto orig = r.origin()[i];

            auto t0 = (axis(i).min - orig) * inv_d;
            auto t1 = (axis(i).max - orig) * inv_d;

            if(inv_d < 0)
                std::swap(t0, t1);
            
            if(t0 > ray_t.min)  ray_t.min = t0;
            if(t1 < ray_t.max)  ray_t.max = t1;

            if(ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
private:
    interval x, y, z;
};

#endif