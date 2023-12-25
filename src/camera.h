#ifndef CAMERA_H
#define CAMERA_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "include/stb_image_write.h"

#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable_list.h"
#include "util.h"

class camera {
public:
    // render
    void render(const hittable_list& world) {
        initialize();

        uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * image_height * image_width * channels);

        for(int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
            for(int i = 0; i < image_width; ++i) {
                auto pixel = pixelloc_00 + pixel_delta_u * i + pixel_delta_v * j;
                ray r(pixel - camera_center, camera_center);

                color pixel_color = ray_color(r, world);
                write_color(data, pixel_color, image_width, channels, i, j);
            };
        }
        std::clog << "\rDone.                        \n";
        stbi_write_jpg(out, image_width, image_height, 3, data, 100);
    }
private:
    // image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    int image_height;

    const int channels = 3;
    const char *out = "render.jpg";

    // camera
    double focal_length;
    double viewport_height;
    double viewport_width;
    point3 camera_center;

    vec3 viewport_u;
    vec3 viewport_v;

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    point3 viewport_upper_left;
    point3 pixelloc_00;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = image_height > 1 ? image_height : 1;

        focal_length = 1.0;
        viewport_height = 2.0;
        viewport_width = viewport_height * static_cast<double>(aspect_ratio);
        camera_center = point3(0, 0, 0);

        // viewport axis vectors
        viewport_u = vec3(viewport_width, 0, 0);
        viewport_v = vec3(0, -viewport_height, 0);

        // pixel delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // upper-left pixel location
        viewport_upper_left = camera_center - point3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixelloc_00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable_list& world) {
        hit_record rec;

        if(world.hit(r, 0, infinity, rec)) {
            return 0.5 * (color(rec.N) + color(1, 1, 1));
        }

        auto a = 0.5 * (unit_vector(r.direction()).y() + 1);
        return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif