#ifndef VEC3_HPP
#define VEC3_HPP

#include <stdexcept>
#include <cmath>
#include "formatter.cpp"
#include "constants.hpp"

// class IndexError: public std::exception {
// public:
//     IndexError(const std::string &what): std::exception(), (what.c_str()){}
// }
class Vec3 {
    double e[3]; // three element vector
public:
    // constructors
    Vec3(): e{0,0,0} {}
    Vec3(double e0, double e1, double e2): e{e0, e1, e2}{}
    Vec3(double t): e{t, t, t}{} // broadcasting

    // access
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    double operator[](int i) const {
        if (i < 0 || i >= 3) throw std::out_of_range(format("index % not in {0,1,2}", i));
        return e[i];
    }
    double &operator[](int i) {
        if (i < 0 || i >= 3) throw std::out_of_range(format("index % not in {0,1,2}", i));
        return e[i];
    }

    // length
    double squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    double length() const {
        return std::sqrt(squared_length());
    }
    double sum() const {
        return e[0] + e[1] + e[2];
    }

    // op overload
    Vec3& operator=(const Vec3 &right) {
        e[0] = right[0];
        e[1] = right[1];
        e[2] = right[2];
        return *this;
    }
    Vec3& operator+=(const Vec3 &right) {
        e[0] += right[0];
        e[1] += right[1];
        e[2] += right[2];
        return *this;
    }
    Vec3& operator-=(const Vec3 &right) {
        e[0] -= right[0];
        e[1] -= right[1];
        e[2] -= right[2];
        return *this;
    }
    Vec3& operator*=(const Vec3 &right) {
        e[0] *= right[0];
        e[1] *= right[1];
        e[2] *= right[2];
        return *this;
    }
    Vec3& operator/=(const Vec3 &right) {
        e[0] /= (right[0]+EPS);
        e[1] /= (right[1]+EPS);
        e[2] /= (right[2]+EPS);
        return *this;
    }
};

// symmetric operator overloads
inline Vec3 operator+(const Vec3 &left, const Vec3 &right) {
    return {left[0] + right[0], left[1] + right[1], left[2] + right[2]};
}
inline Vec3 operator-(const Vec3 &left, const Vec3 &right) {
    return {left[0] - right[0], left[1] - right[1], left[2] - right[2]};
}
inline Vec3 operator*(const Vec3 &left, const Vec3 &right) {
    return {left[0] * right[0], left[1] * right[1], left[2] * right[2]};
}
inline Vec3 operator/(const Vec3 &left, const Vec3 &right) {
    return {left[0] / (right[0]+EPS), left[1] / (right[1]+EPS), left[2] / (right[2]+EPS)};
}
inline Vec3 operator-(const Vec3 &right) { 
    return {-right[0], -right[1], -right[2]}; 
}

// equality operators
inline bool operator==(const Vec3 &left, const Vec3 &right) {
    return (left - right).squared_length() < EPS;
}
inline bool operator!=(const Vec3 &left, const Vec3 &right) {
    return !(left == right);
}


// vector operations
inline double dot(const Vec3 &left, const Vec3 &right) {
    return (left * right).sum();
}
inline Vec3 cross(const Vec3 &left, const Vec3 &right) {
    return {
        left[1] * right[2] - left[2] * right[1],
        left[2] * right[0] - left[0] * right[2],
        left[0] * right[1] - left[1] * right[0]
    };
}
inline Vec3 unit_vector(const Vec3 &v) {
    return v/v.length();
}

// type aliases
using point3d = Vec3;
using RGBcolor = Vec3;

// optic algebra
Vec3 reflect(const Vec3 &reflected, const Vec3 &normal) {
    return reflected - 2 * dot(reflected, normal) * normal;
}

Vec3 refract(const Vec3 &reflected, const Vec3 &normal, double refr_index) {
    Vec3 parallel = (reflected - dot(reflected, normal) * normal)/(refr_index * reflected.length());
    if (parallel.squared_length() < 1 - EPS) {
        Vec3 perp = -std::sqrt(1 - parallel.squared_length()) * normal;
        return (parallel + perp);
    }
    return reflect(reflected, normal); // TIR
}

#endif