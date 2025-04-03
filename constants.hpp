#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "vec3.hpp"

// Image constants
const double aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);

// camera constants
const double viewport_height = 2.0; // height of viewing hole/camera screen
const double viewport_width = aspect_ratio * viewport_height; // same aspect ratio as image
const double focal_length = 1.0; // distance from the eye/camera lens

// other constants
const double EPS = 1e-08;
const double PI  = 3.1415926535897932385;

#endif