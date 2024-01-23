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
    cam.look_from = point3(-2, 2, 1);
    cam.look_at   = point3(0, 0, -1);

    cam.render(scene);
}