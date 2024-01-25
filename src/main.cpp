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

void spheres()
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
    scene.add(std::make_shared<quad>(point3(-100.0, -0.5, 100.0), vec3(200.0, 0.0, 0.0), vec3(0.0, 0.0, -200.0), checker_material));
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

    cam.background = color(0.70, 0.80, 1.00);

    cam.render(scene);
}

void cornell_box()
{
    hittable_list world;

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    world.add(std::make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(std::make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(std::make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(std::make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(std::make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(std::make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    std::shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    std::shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);
    
    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples = 50;
    cam.max_depth = 50;
    cam.background = color(0,0,0);

    cam.vfov = 40;
    cam.look_from = point3(278, 278, -800);
    cam.look_at = point3(278, 278, 0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main(int argc, char* argv[])
{
    int c = 2;
    if(argc > 1)
        c = std::stoi(argv[1]);
    
    switch(c) {
        case 1: spheres(); break;
        case 2: cornell_box();   break;
    }
}