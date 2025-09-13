#include "Framebuffer.h"
#include "colour.h"
#include "Point.h"
#include <vector>

Framebuffer::Framebuffer(int h, int w)
    : height{ h }, width{ w }, pixels(h * w)
{
    init_default();
}

void Framebuffer::init_default() {
    for (int y = 0; y < height; y++) {
        float b = 0.5f;
        std::clog << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
        float g = static_cast<float>(y) / height;

        for (int x = 0; x < width; x++) {
            float r = static_cast<float>(x) / width;

            const colour pixel_colour(
                static_cast<int>(255.999 * r),
                static_cast<int>(255.999 * g),
                static_cast<int>(255.999 * b)
            );

            set_pixel_colour(pixel_colour, Point(x, y));
        }
    }
}

void Framebuffer::set_pixel_colour(const colour pixel_colour, const Point p) {
    const int row_offset = p.y * width;
    pixels[row_offset + p.x] = pixel_colour;
}
