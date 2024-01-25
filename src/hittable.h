#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

class material;

class hit_record {
public:
    vec3 N;
    point3 p;
    double t;
    std::shared_ptr<material> mat;
    bool front_face;
    double u;
    double v;

    void set_normal_face(const ray& r, const vec3& outward_normal) {
        front_face = dot(outward_normal, r.direction()) < 0;
        N = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
virtual ~hittable() = default;
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
public:
    translate(std::shared_ptr<hittable> _object, const vec3& _offset)
        : object(_object), offset(_offset) {
        bbox = object->bounding_box() + offset;
    }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        ray offset_r(r.direction(), r.origin() - offset);
        
        if(!object->hit(offset_r, t_min, t_max, rec))
            return false;

        rec.p += offset;
        
        return true;
    }

    aabb bounding_box() const override { return bbox; }
private:
    std::shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

class rotate_y : public hittable {
public:
    rotate_y(std::shared_ptr<hittable> _object, double angle)
        : object(_object) {
            auto radians = degree_to_rad(angle);
            sin_theta = sin(radians);
            cos_theta = cos(radians);
            bbox = object->bounding_box();

            point3 min( infinity,  infinity,  infinity);
            point3 max(-infinity, -infinity, -infinity);

            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 2; ++j) {
                    for(int k = 0; k < 2; ++k) {
                        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;
                    
                        auto newx =  cos_theta * x + sin_theta * z;
                        auto newz = -sin_theta * x + cos_theta * z;

                        vec3 tester(newx, y, newz);

                        for(int c = 0; c < 3; ++c) {
                            min[c] = fmin(min[c], tester[c]);
                            max[c] = fmax(max[c], tester[c]);
                        }
                    }
                }
            }
            bbox = aabb(min, max);
        }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        auto origin = r.origin();
        auto direction = r.direction();

        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        ray rotated_r(direction, origin);

        if(!object->hit(rotated_r, t_min, t_max, rec))
            return false;

        // change intersectiom point from object space to world space
        auto p = rec.p;
        p[0] =  cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        // change normal from object space to world space
        auto normal = rec.N;
        normal[0] =  cos_theta * rec.N[0] + sin_theta * rec.N[2];
        normal[2] = -sin_theta * rec.N[0] + cos_theta * rec.N[2];

        rec.p = p;
        rec.N = normal;

        return true;
    }

    aabb bounding_box() const override { return bbox; }
private:
    std::shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};

#endif