#include <iostream>
#include <memory>

#include "camera.h"
#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "color.h"

int main(int argc, char* argv[])
{
    // World
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<glass>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);

    // RHS, X is horizontal, Y is vertical and Z goes through the screen
    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    camera cam;
    cam.look_from = point3(-2,2,1);
    cam.look_at   = point3(0,0,-1);

    cam.render(world);
}