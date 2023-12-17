#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "include/stb_image_write.h"

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"

const int image_height { 800 };
const int image_width { 800 };

const int channels { 3 };

const char *out = "render.jpg";

int main(int argc, char* argv[])
{
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * image_height * image_width * channels);

    for(int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
        for(int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0;

            auto pixel_color = color(r, g, b);
            write_color(data, pixel_color, image_width, channels, i, j);
        };
    }
    std::clog << "\rDone.                        \n";
    stbi_write_jpg(out, image_width, image_height, 3, data, 100);
}