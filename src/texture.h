#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "color.h"
#include "img_reader.h"

#include <memory>

class texture {
public:
    virtual ~texture() = default;
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid : public texture {
public:
    solid(color _c)
        : c(_c) {}
    solid(double r, double g, double b)
        : c(color(r, g, b)) {}

    color value(double u, double v, const point3& p) const override {
        return c;
    }
private:
    color c;
};

class checker : public texture {
public:
    checker(double _scale, std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd)
        : iscale(1.0 / _scale), even(_even), odd(_odd) {}
    checker(double _scale, color c1, color c2)
        : iscale(1.0 / _scale),
          even(std::make_shared<solid>(c1)),
          odd(std::make_shared<solid>(c2)) {}

    color value(double u, double v, const point3& p) const override {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        auto x_int = static_cast<int>(std::floor(iscale * p.x()));
        auto y_int = static_cast<int>(std::floor(iscale * p.y()));
        auto z_int = static_cast<int>(std::floor(iscale * p.z()));

        bool is_even = (x_int + y_int + z_int) % 2 == 0;
        return is_even ? even->value(u, v, p) : odd->value(u, v, p);
    }
private:
    double iscale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class image : public texture {
public:
    image(const char* file) : img(file) {}

    color value(double u, double v, const point3& p) const override {
        if(img.height() <= 0) return color(0,1,1);

        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);

        auto i = static_cast<int>(u * img.width());
        auto j = static_cast<int>(v * img.height());
        auto pixel = img.pixel_data(i,j);

        auto scale = 1.0 / 255.0;
        return color(scale * pixel[0], scale * pixel[1], scale * pixel[2]);
    }
private:
    image_reader img;
};

#endif