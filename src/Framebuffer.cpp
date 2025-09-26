#include "Framebuffer.h"
#include "Colour.h"
#include "Point.h"

#include <cassert>
#include <vector>
#include <limits>

Framebuffer::Framebuffer(int w, int h)
    : m_width{ w }, m_height{ h }
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

    pixels.resize(m_width * m_height);
}

void Framebuffer::InitDefault() {
    constexpr float b{ 0.5f };
    for (int y = 0; y < m_height; y++) {
        std::clog << "\rScanlines remaining: " << (m_height - y) << ' ' << std::flush;
        float g{ static_cast<float>(y) / m_height };

        for (int x = 0; x < m_width; x++) {
            float r{ static_cast<float>(x) / m_width };

            const Colour pixel_colour(
                static_cast<int>(255.999 * r),
                static_cast<int>(255.999 * g),
                static_cast<int>(255.999 * b)
            );

            SetPixelColour(pixel_colour, Point(x, y));
        }
    }
}

void Framebuffer::SetPixelColour(const Colour& colour, const Point& p) {
    assert(p.x < m_width && "p.x must be in range of framebuffer width");
    assert(p.y < m_height && "p.y must be in range of framebuffer height");

    const int row_offset{ p.y * m_width };
    pixels[row_offset + p.x] = colour;
}
