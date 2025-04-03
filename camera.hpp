#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "essentials.hpp"
#include <cmath>
#include <iostream>
std::ostream &operator<<(std::ostream &o, const Vec3 &v) {
    o << '[' << v[0] << " " << v[1] << " " << v[2] << "]\n";
    return o;
}

class Camera {
    // essential points and directions
    Vec3 horizontal; // the horizontal direction
    Vec3 vertical; // the vertical direction

    point3d origin; // the eye
    point3d lower_left_corner; // lower left corner of the screen. add horiz/vert/horiz+vert to get the other corners

    Vec3 x_dir, y_dir, z_dir;

    double aperture;
public:
    Camera (
        const point3d &lookfrom, // origin
        const point3d &lookat, // center of image to look at (center of focus plane)
        const Vec3 &camera_up,   // defines when the camera is "horizontal"
        double aspect_ratio, 
        double viewport_vert_fov, // vertical fov (degrees) 
        double focal_dist, // distance from camera lens to focus plane
        double aperture    // the diameter of the camera lens. this lens focuses light from focus plane/scene to image plane/screen
    ): aperture(aperture) {
        origin = lookfrom;
        double theta = PI * viewport_vert_fov/180;
        double viewport_height = 2 * std::tan(theta/2);
        double viewport_width  = viewport_height * aspect_ratio;

        z_dir = unit_vector(lookfrom - lookat);
        x_dir = unit_vector(cross(camera_up, z_dir));
        y_dir = cross(z_dir, x_dir);

        // horizontal and vertical vectors representing the portion of the scene we will capture
        horizontal = focal_dist * viewport_width  * x_dir;
        vertical   = focal_dist * viewport_height * y_dir;

        lower_left_corner = lookfrom - focal_dist * z_dir - horizontal/2 - vertical/2;
        // std::cout << origin << std::endl;
        // std::cout << horiz_vec << std::endl;
        // std::cout << horizontal << std::endl;
        // std::cout << vertical << std::endl;
        // std::cout << lower_left_corner << std::endl;
    }

    // parameterize the screen by [0,1]x[0,1]
    Ray get_ray(double u, double v) const {
        auto [radius, theta] = sampler.random_polar_in_unit_disk();
        radius *= aperture/2; // sample from the lens uniformly
        point3d ray_start_point = origin + radius * std::cos(theta) * x_dir + radius * std::sin(theta) * y_dir;
        point3d ray_end_point = lower_left_corner + u*horizontal + v*vertical; 
        return {ray_start_point, ray_end_point - ray_start_point};
    }
};


#endif