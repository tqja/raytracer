#include "Framebuffer.h"
#include "Colour.h"
#include "Point.h"
#include <vector>
#include <limits>

Framebuffer::Framebuffer(int w, int h)
    : width{ w }, height{ h }
{
    if (h <= 0) {
        throw std::invalid_argument("Framebuffer height must be greater than 0");
    }

    if (w <= 0) {
        throw std::invalid_argument("Framebuffer width must be greater than 0");
    }

    if (h > max_height) {
        throw std::invalid_argument("Framebuffer height must be less than max_height");
    }

    if (w > max_width) {
        throw std::invalid_argument("Framebuffer width must be less than max_width");
    }

    pixels.resize(width * height);
}

void Framebuffer::InitDefault() {
    for (int y = 0; y < height; y++) {
        float b = 0.5f;
        std::clog << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
        float g = static_cast<float>(y) / height;

        for (int x = 0; x < width; x++) {
            float r = static_cast<float>(x) / width;

            const Colour pixel_colour(
                static_cast<int>(255.999 * r),
                static_cast<int>(255.999 * g),
                static_cast<int>(255.999 * b)
            );

            SetPixelColour(pixel_colour, Point(x, y));
        }
    }
}

void Framebuffer::SetPixelColour(const Colour pixel_colour, const Point p) {
    const int row_offset = p.y * width;
    pixels[row_offset + p.x] = pixel_colour;
}
