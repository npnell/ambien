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
    double aspect_ratio = 1.0;
    int image_width = 400;
    int samples = 10;
    int max_depth = 50;

    double vfov = 80.0;

    point3 look_from = point3(0, 0, 9);
    point3 look_at   = point3(0, 0, 0);
    vec3   vup       = vec3(0, 1, 0);

    double defocus_angle = 0.0;
    double focus_dist = 10;

    int sqrt_spp;
    double recip_sqrt_spp;

    color background;

    // render
    void render(const hittable_list& world) {
        initialize();

        uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * image_height * image_width * channels);

        for(int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
            for(int i = 0; i < image_width; ++i) {
                color pixel_color = color(0,0,0);

                for(int s_j = 0; s_j < sqrt_spp; ++s_j) {
                    for(int s_i = 0; s_i < sqrt_spp; ++s_i) {
                        ray r = get_ray(i, j, s_i, s_j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
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

    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        sqrt_spp = static_cast<int>(sqrt(samples));
        recip_sqrt_spp = 1.0 / sqrt_spp;

        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = image_height > 1 ? image_height : 1;

        auto theta = degree_to_rad(vfov);
        auto h = tan(theta / 2.0);
        double viewport_height = 2 * h * focus_dist;
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
        point3 viewport_upper_left = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixelloc_00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * tan(degree_to_rad(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    color ray_color(const ray& r, int depth, const hittable_list& world) const {
        hit_record rec;

        if(depth <= 0.0) {
            return color(0,0,0);
        }

        if(!world.hit(r, 0.001, infinity, rec))
            return background;

        ray scattered;
        color attenuation;
        color emission_color = rec.mat->emitted(rec.u, rec.v, rec.p);

        if(!rec.mat->scatter(r, rec, attenuation, scattered))
            return emission_color;
        
        color scatter_color = attenuation * ray_color(scattered, depth - 1, world);

        return emission_color + scatter_color;
    }

    ray get_ray(int i, int j, int s_i, int s_j) const {
        auto pixel_center = pixelloc_00 + pixel_delta_u * i + pixel_delta_v * j;
        auto pixel_sample = pixel_center + pixel_sample_square(s_i, s_j);

        auto ray_origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin ;
        return ray(ray_direction, ray_origin);
    }

    vec3 pixel_sample_square(int s_i, int s_j) const {
        auto px = -0.5 + recip_sqrt_spp * (s_i * random_double());
        auto py = -0.5 + recip_sqrt_spp * (s_j * random_double());

        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};

#endif