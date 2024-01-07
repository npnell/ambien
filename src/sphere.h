#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
public:
    sphere(const point3& _center, double _radius, std::shared_ptr<material> _mat) : center(_center), radius(_radius), mat(_mat) {}
    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        auto a = dot(r.direction(), r.direction());
        auto half_b = dot(r.direction(), r.origin() - center);
        auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;

        auto discriminant = half_b * half_b - a * c;

        if (discriminant <= 0) {
            return false;
        }

        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a;
        if(root <= t_min || t_max <= root) {
            root = (-half_b + sqrtd) / a;
            if(root <= t_min || t_max <= root) {
                return false;
            }
        }

        rec.p = r.at(root);
        rec.t = root;
        rec.mat = mat;
        
        vec3 N = (rec.p - center) / radius;
        rec.set_normal_face(r, N);

        return true;
    }
private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif