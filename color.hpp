#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "essentials.hpp"
#include <iostream>

// write a pixel to ppm file
void write_color(std::ostream &ost, const RGBcolor &pixel) {
    ost \
    << int(256 * clamp(std::sqrt(pixel[0]), 0.0, 0.999)) << " " \
    << int(256 * clamp(std::sqrt(pixel[1]), 0.0, 0.999)) << " " \
    << int(256 * clamp(std::sqrt(pixel[2]), 0.0, 0.999)) << "\n";
}

#endif