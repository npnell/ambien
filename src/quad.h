#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

#include <cmath>

class quad : public hittable {
public:
    quad(const point3& _Q, const vec3& _u, const vec3& _v, std::shared_ptr<material> _mat)
        : Q(_Q), u(_u), v(_v), mat(_mat)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
        set_bounding_box();
    }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        if(fabs(denom) < 1e-8)
            return false;
        
        auto t = (D - dot(normal, r.origin())) / denom;
        if(t < t_min || t > t_max)
            return false;

        auto intersection = r.at(t);
        vec3 planar_hitpt_vec = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vec, v));
        auto beta = dot(w, cross(u, planar_hitpt_vec));

        if(!is_interior(alpha, beta, rec))
            return false;

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_normal_face(r, normal);

        return true;
    }

    aabb bounding_box() const override { return bbox; }

    void set_bounding_box() {
        bbox = aabb(Q, Q + u + v).pad();
    }

    bool is_interior(double a, double b, hit_record& rec) const {
        if (a < 0 || a > 1 || b < 0 || b > 1)
            return false;
        
        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    std::shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
    vec3 w;
};

#endif