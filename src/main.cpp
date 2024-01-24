#include <iostream>
#include <memory>
#include <string>

#include "camera.h"
#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "color.h"
#include "bvh.h"
#include "quad.h"

void spheres_scene()
{
    // World
    hittable_list scene;

    auto material_left   = std::make_shared<glass>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.01);
    auto earth_texture   = std::make_shared<image>("../assets/earthmap.jpg");
    auto earth_material  = std::make_shared<lambertian>(earth_texture);
    auto checker_texture = std::make_shared<checker>(0.32, color(0,0,0), color(1,1,1));
    auto checker_material = std::make_shared<lambertian>(checker_texture);

    // RHS, X is horizontal, Y is vertical and Z goes through the screen
    scene.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, checker_material));
    scene.add(std::make_shared<sphere>(point3( 0.0, 0.0, -1.0), 0.5, earth_material));
    scene.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    scene.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    scene.add(std::make_shared<sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    scene = hittable_list(std::make_shared<bvh_node>(scene));
    
    // Camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.look_from = point3(-2, 2, 1);
    cam.look_at   = point3(0, 0, -1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 3.4;

    cam.render(scene);
}

void quads_scene()
{
    hittable_list scene;

    // Materials
    auto left_red     = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    scene.add(std::make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    scene.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    scene.add(std::make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    scene.add(std::make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    scene.add(std::make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples = 10;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.look_from = point3(0,0,9);
    cam.look_at = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(scene);
}

int main(int argc, char* argv[])
{
    int c = std::stoi(argv[1]);
    switch(c) {
        case 1: spheres_scene(); break;
        case 2: quads_scene();   break;
    }
}