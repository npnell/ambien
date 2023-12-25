#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

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

#endif