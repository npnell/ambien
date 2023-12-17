#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using color = vec3;

void write_color(uint8_t *data, const color& pixel_color, int image_width, int channels, int i, int j)
{
    auto ir = static_cast<uint8_t>(pixel_color[0] * 255.99);
    auto ig = static_cast<uint8_t>(pixel_color[1] * 255.99);
    auto ib = static_cast<uint8_t>(pixel_color[2] * 255.99);
    
    data[j * image_width * channels + i * channels] = ir;
    data[j * image_width * channels + i * channels + 1] = ig;
    data[j * image_width * channels + i * channels + 2] = ib;
}

#endif