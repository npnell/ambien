#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "texture.h"

class hit_record;

class material {
public:
    virtual ~material() = default;
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& _albedo) : albedo(std::make_shared<solid>(_albedo)) {}
    lambertian(std::shared_ptr<texture> _albedo) : albedo(_albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const override {
        auto scatter_dir = rec.N + random_unit_vector();
        
        if(scatter_dir.near_zero())
            scatter_dir = rec.N;
        
        r_scatter = ray(scatter_dir, rec.p);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
private:
    std::shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz < 1 ? _fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const override {
        auto reflected_dir = reflect(unit_vector(r_in.direction()), rec.N);
        r_scatter = ray(reflected_dir + fuzz * random_in_unit_sphere(), rec.p);
        attenuation = albedo;
        return (dot(r_scatter.direction(), rec.N) > 0);
    }
private:
    color albedo;
    double fuzz;
};

class glass : public material {
public:
    glass(double _ir) : ir(_ir) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& r_scatter) const override {
        attenuation = color(1.0, 1.0, 1.0);
        
        auto unit_dir = unit_vector(r_in.direction());
        auto cos_theta = fmin(dot(-unit_dir, rec.N), 1.0);
        auto sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        auto rr = rec.front_face ? 1.0 / ir : ir;
        
        vec3 dir;
        if(rr * sin_theta > 1.0 || reflectance(cos_theta, rr) > random_double()) {
            dir = reflect(unit_dir, rec.N);
        } else {
            dir = refract(unit_dir, rec.N, rr);
        }

        r_scatter = ray(dir, rec.p);
        return true;
    }
private:
    double ir;

    static double reflectance(double cos, double ir) {
        auto r0 = (1 - ir) / (1 + ir);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cos), 5);
    }
};

#endif