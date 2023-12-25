#include <iostream>
#include <memory>

#include "camera.h"
#include "sphere.h"
#include "hittable_list.h"


int main(int argc, char* argv[])
{
    //world
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -2.0), 1));
    world.add(std::make_shared<sphere>(point3(0, -101, -2.0), 100));

    // camera
    camera cam;
    cam.render(world);   
}