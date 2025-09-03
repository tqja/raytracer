#include <algorithm>
#include <iostream>
#include <vector>
#include "stb_image_write.h"

void write_framebuffer_to_png(const char* filename, const int width, const int height) {
    constexpr int channels{ 3 };
    std::vector<uint8_t> pixels(width * height * channels);

    int index = 0;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float r = (float)i / (float)width;
            float g = (float)j / (float)height;
            float b = 0.0f;

            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    stbi_write_png(filename, width, height, channels, pixels.data(), width * channels);
}
 

int main()
{
    constexpr float aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 1920;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
    static_assert(image_height > 1, "image_height must be greater than 1");
    write_framebuffer_to_png("image.png", image_width, image_height);

}