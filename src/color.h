#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double c) {
    return sqrt(c);
}

void write_color(uint8_t *data, const color& pixel_color, int image_width, int channels, int i, int j, int samples)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples;
    r *= scale;
    g *= scale;
    b *= scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);
    data[j * image_width * channels + i * channels] = static_cast<uint8_t>(256 * intensity.clamp(r));
    data[j * image_width * channels + i * channels + 1] = static_cast<uint8_t>(256 * intensity.clamp(g));
    data[j * image_width * channels + i * channels + 2] = static_cast<uint8_t>(256 * intensity.clamp(b));
}

#endif