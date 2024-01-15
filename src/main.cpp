#include <iostream>
#include <memory>

#include "camera.h"
#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "color.h"
#include "bvh.h"

int main(int argc, char* argv[])
{
    // World
    hittable_list scene;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<glass>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.01);

    // RHS, X is horizontal, Y is vertical and Z goes through the screen
    scene.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    scene.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    scene.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    scene.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    scene.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    scene = hittable_list(std::make_shared<bvh_node>(scene));
    
    // Camera
    camera cam;
    cam.look_from = point3(-2,2,1);
    cam.look_at   = point3(0,0,-1);

    cam.render(scene);
}