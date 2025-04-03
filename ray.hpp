#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"
class Ray {
    point3d orig;
    Vec3 dir;
public:
    // constructors
    Ray() {}
    Ray(const point3d &origin, const Vec3 &dir): orig(origin), dir(dir){}

    // access
    const point3d &origin() const { return orig; }
    const Vec3 &direction() const { return dir; }

    // operation
    Vec3 at(double t) const {
        return orig + t * dir;
    }
};

#endif