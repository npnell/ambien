#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "include/stb_image_write.h"

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"
#include "ray.h"

const int image_width { 400 };

const int channels { 3 };

const char *out = "render.jpg";

color ray_color(const ray& r)
{
    auto a = 0.5 * (unit_vector(r.direction()).y() + 1);
    return a * color(1.0, 1.0, 1.0) + (1 - a) * color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    // image
    const auto aspect_ratio = 16.0 / 9.0;

    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = image_height > 1 ? image_height : 1;

    // camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * static_cast<double>(aspect_ratio);
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - point3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixelloc_00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // render
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * image_height * image_width * channels);

    for(int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
        for(int i = 0; i < image_width; ++i) {
            /*
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0;
            */

            auto pixel = pixelloc_00 + pixel_delta_u * i + pixel_delta_v * j;
            ray r(pixel - camera_center, camera_center);

            color pixel_color = ray_color(r);
            write_color(data, pixel_color, image_width, channels, i, j);
        };
    }
    std::clog << "\rDone.                        \n";
    stbi_write_jpg(out, image_width, image_height, 3, data, 100);
}