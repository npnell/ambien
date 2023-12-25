#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
public:
    vec3 N;
    point3 p;
    double t;

    void set_normal_face(const ray& r, const vec3& outward_normal) {
        front_face = dot(outward_normal, r.direction()) < 0;
        N = front_face ? outward_normal : -outward_normal;
    }
private:
    bool front_face;
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hit_record) const = 0;
};

#endif