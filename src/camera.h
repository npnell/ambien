#ifndef CAMERA_H
#define CAMERA_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "include/stb_image_write.h"

#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "util.h"
#include "material.h"

class camera {
public:
    // render
    void render(const hittable_list& world) {
        initialize();

        uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * image_height * image_width * channels);

        for(int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
            for(int i = 0; i < image_width; ++i) {
                

                color pixel_color = color(0,0,0);

                for(int s = 0; s < samples; ++s) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(data, pixel_color, image_width, channels, i, j, samples);
            };
        }
        std::clog << "\rDone.                        \n";
        stbi_write_jpg(out, image_width, image_height, 3, data, 100);

        free(data);
    }
private:
    // image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    int image_height;
    int samples = 100;
    int max_depth = 50;

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
        viewport_width = viewport_height * aspect_ratio;
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

    color ray_color(const ray& r, int depth, const hittable_list& world) {
        hit_record rec;

        if(depth <= 0.0) {
            return color(0,0,0);
        }

        if(world.hit(r, 0.001, infinity, rec)) {
            ray scattered;
            color attenuation;

            if(rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0,0,0);
        }

        auto a = 0.5 * (unit_vector(r.direction()).y() + 1.0);
        return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) const {
        auto pixel_center = pixelloc_00 + pixel_delta_u * i + pixel_delta_v * j;
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin ;
        return ray(ray_direction, ray_origin);
    }

    vec3 pixel_sample_square() const {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();

        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};

#endif