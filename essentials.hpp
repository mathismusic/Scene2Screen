#ifndef ESSENTIALS_HPP
#define ESSENTIALS_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "constants.hpp"

// essential points and directions

const Vec3 horizontal = Vec3(viewport_width, 0, 0); // the horizontal direction
const Vec3 vertical = Vec3(0, viewport_height, 0); // the vertical direction

const point3d origin = point3d(0, 0, 0); // the eye
const point3d lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length); // lower left corner of the screen. add horiz/vert/horiz+vert to get the other corners

// colors
const RGBcolor WHITE = RGBcolor(1.0, 1.0, 1.0);
const RGBcolor RED   = RGBcolor(1.0, 0.0, 0.0);
const RGBcolor GREEN = RGBcolor(0.0, 1.0, 0.0);
const RGBcolor BLUE  = RGBcolor(0.0, 0.0, 1.0);

class Sampler {
public:
    Sampler() {}
    double random_double() {
        // Returns a random real in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }
    double random_double(double min, double max) {
        // Returns a random real in [min,max).
        return min + (max-min)*random_double();
    }

    Vec3 random_vector(double min, double max) {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
    Vec3 random_unit_vector() {
        return unit_vector(random_vector(0., 1.));
    }
    std::pair<double, double> random_polar_in_unit_disk() {
        double theta = random_double() * 2 * PI;
        double radius = std::sqrt(random_double());
        return {radius, theta};
    }
};
Sampler sampler;

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


#endif