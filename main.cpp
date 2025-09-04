#include <algorithm>
#include <iostream>
#include <vector>

#include "colour.h"
#include "vec3.h"
#include "stb_image_write.h"


void write_framebuffer_to_png(const char* filename, const int width, const int height, std::vector<vec3>& framebuffer) {
    constexpr int channels{ 3 };
    std::vector<uint8_t> image(width * height * channels);

    for (int i = 0; i < framebuffer.size(); i++) {
        for (int colour = 0; colour < channels; colour++) {
            image[i * channels + colour] =
                static_cast<uint8_t>(framebuffer[i][colour]);
        }
    }

    stbi_write_png(filename, width, height, channels, image.data(), width * channels);
}

std::vector<vec3> init_framebuffer(const int width, const int height) {
    std::vector<vec3> framebuffer(width * height);

    int index{};

    for (int y = 0; y < height; y++) {
        std::clog << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
        for (int x = 0; x < width; x++) {
            float r = (float)x / (float)width;
            float g = (float)y / (float)height;
            float b = 0.0f;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            const colour pixel_colour = vec3(ir, ig, ib);
            write_pixel_to_framebuffer(framebuffer, pixel_colour, x, y, width);
        }
    }

    return framebuffer;
}

int main()
{
    constexpr float aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 1920;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
    static_assert(image_height > 1, "image_height must be greater than 1");

    std::vector<vec3> framebuffer{ init_framebuffer(image_width, image_height) };
    write_framebuffer_to_png("image.png", image_width, image_height, framebuffer);

    return 0;
}