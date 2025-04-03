#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "essentials.hpp"

class Material;

struct hit_record {
    point3d p;
    Vec3 normal; // surface normal (against the incident ray).
    double t; // perhaps useful sometimes.

    // what's the material of the hit
    std::shared_ptr<Material> mat_ptr;
    
    bool front_face;
    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < -EPS;
        normal = front_face?outward_normal:-outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual ~hittable(){}
};



#endif