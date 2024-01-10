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
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples = 100;
    int max_depth = 50;

    double vfov = 20.0;

    point3 look_from = point3(0, 0, -1);
    point3 look_at   = point3(0, 0, 0);
    vec3   vup       = vec3(0, 1, 0);

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
    int image_height;

    const int channels = 3;
    const char *out = "render.jpg";

    // camera
    point3 camera_center;

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    point3 pixelloc_00;
    
    vec3 u, v, w;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = image_height > 1 ? image_height : 1;

        auto theta = degree_to_rad(vfov);
        auto h = tan(theta / 2.0);

        double focal_length = (look_at - look_from).len();
        double viewport_height = 2 * h * focal_length;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
        camera_center = look_from;

        // basis vectors
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // viewport axis vectors
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // pixel delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // upper-left pixel location
        point3 viewport_upper_left = camera_center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
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