#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
public:
    sphere(const point3& _center, double _radius, std::shared_ptr<material> _mat)
        : center(_center), radius(_radius), mat(_mat) {
            auto rvec = vec3(radius, radius, radius);
            bbox = aabb(center + rvec, center - rvec);
    }

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
        get_uv(N, rec.u, rec.v);

        return true;
    }

    static void get_uv(const point3& p, double& u, double& v) {
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }

    aabb bounding_box() const override { return bbox; }
private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
    aabb bbox;
};

#endif