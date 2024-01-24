#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

#include "util.h"

class vec3 {
public:
    vec3() : e{0,0,0} {}
    vec3(double x, double y, double z) : e{x, y, z} {}
    
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    double len() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    bool near_zero() const {
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
private:
    double e[3];
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3& u) {
    return vec3(t * u[0], t * u[1], t * u[2]);
}

inline vec3 operator*(const vec3& u, double t) {
    return t * u;
}

inline vec3 operator/(const vec3& u, double t) {
    return double((1 / t)) * u;
}

inline double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vector(const vec3& u) {
    return u / u.len();
}

inline vec3 random_vector() {
    return vec3(random_double(), random_double(), random_double());
}

inline vec3 random_vector(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 random_in_unit_disk() {
    while(true) {
        auto _r = vec3(random_double(-1,1), random_double(-1,1), 0);
        if(dot(_r,_r) < 1)
            return _r;
    }
}

vec3 random_in_unit_sphere() {
    while(true) {
        vec3 _r = random_vector(-1,1);
        if(dot(_r,_r) < 1)
            return _r;
    }
}

inline vec3 random_unit_vector() { 
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    auto _r = random_unit_vector();
    return dot(_r, normal) > 0.0 ? _r : -_r;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

vec3 refract(const vec3& R, const vec3& n, double coeff) {
    auto cos_theta = fmin(dot(-R,n), 1.0);
    auto R_perp = coeff * (R + cos_theta * n);
    auto R_paral = -sqrt(fabs(1.0 - dot(R_perp, R_perp))) * n;
    return R_perp + R_paral;
}

#endif