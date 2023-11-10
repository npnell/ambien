#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "include/stb_image_write.h"

#include <iostream>
#include <fstream>

const int image_width { 256 };
const int image_height { 256 };

const char *out = "render.jpg";

int main(int argc, char* argv[])
{

    uint8_t data[3 * image_width][image_height];

    for(int i = 0; i < image_width; ++i) {
        std::clog << "\rScanlines remaining: " << image_height - i << std::flush;
        for(int j = 0; j < image_height; ++j) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0;

            auto ir = static_cast<uint8_t>(r * 255.99);
            auto ig = static_cast<uint8_t>(g * 255.99);
            auto ib = static_cast<uint8_t>(b * 255.99);
            
            // fstrm << ir << ' ' << ig << ' ' << ib << "\n";
            data[i][j] = ir;
            data[i + 1][j] = ig;
            data[i + 2][j] = ib;
        };
    }
    std::clog << "\rDone.                        \n";
    stbi_write_jpg(out, image_width, image_height, 3, data, 100);
}