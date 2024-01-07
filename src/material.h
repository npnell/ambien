#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "color.h"
#include "vec3.h"

class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const override {
        auto scatter_dir = rec.N + random_unit_vector();
        
        if(scatter_dir.near_zero()) {
            scatter_dir = rec.N;
        }
        r_scatter = ray(scatter_dir, rec.p);
        attenuation = albedo;
        return true;
    }
private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const override {
        auto reflected_dir = reflect(unit_vector(r_in.direction()), rec.N);
        r_scatter = ray(reflected_dir + fuzz * random_unit_vector(), rec.p);
        attenuation = albedo;
        return (dot(r_scatter.direction(), rec.N) > 0);
    }
private:
    color albedo;
    double fuzz;
};

#endif