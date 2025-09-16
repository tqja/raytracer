#include <algorithm>
#include <iostream>
#include <vector>

#include "Framebuffer.h"
#include "colour.h"
#include "vec3.h"
#include "stb_image_write.h"


void WriteFramebufferToPng(const char* filename, const int width, const int height, Framebuffer framebuffer) {
    constexpr int channels{ 3 };
    std::vector<uint8_t> image(width * height * channels);
    std::vector<colour> pixels{ framebuffer.GetFramebuffer() };

    for (int i = 0; i < pixels.size(); i++) {
        for (int colour = 0; colour < channels; colour++) {
            image[i * channels + colour] =
                static_cast<uint8_t>(pixels[i][colour]);
        }
    }

    stbi_write_png(filename, width, height, channels, image.data(), width * channels);
}

int main()
{
    constexpr float aspect_ratio = 16.0f / 9.0f;
    constexpr int image_width = 1920;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
    
    Framebuffer framebuffer{ image_height, image_width };
    framebuffer.InitDefault();

    WriteFramebufferToPng("image.png", image_width, image_height, framebuffer);

    return 0;
}