#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "hittable.hpp"

class Sphere: public hittable {
    point3d cent;
    double rad;
    std::shared_ptr<Material> mat_ptr; // shared_ptr and Material declared in hittable.hpp
public:
    Sphere(): rad(0.) {}
    Sphere(const point3d &center, const double radius, std::shared_ptr<Material> mat_ptr): cent(center), rad(radius), mat_ptr(mat_ptr) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    Vec3 rel = r.origin() - cent;
    double a = r.direction().squared_length();
    double half_b = -dot(rel, r.direction());
    double c = rel.squared_length() - rad*rad;
    double disc = half_b * half_b - a * c;
    if (disc < -EPS) return 0;
    double sqrtD = std::sqrt(disc);
    
    // find the nearest point of intersection that's within the bounds required.
    double t;
    if (double t1 = (half_b - sqrtD)/a; t1 > (t_min + EPS) && t1 < (t_max - EPS)) {
        t = t1;
    } 
    else if (double t2 = (half_b + sqrtD)/a; t2 > (t_min + EPS) && t2 < (t_max - EPS)) {
        t = t2;
    } else { return false; }
    
    rec.p = r.at(t);
    rec.t = t;
    rec.set_face_normal(r, (rec.p - cent)/rad); // if rad is negative, this flips the direction: hollowness
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif