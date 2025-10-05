#include "Framebuffer.h"
#include "Colour.h"
#include "Geometry.h"
#include "Interval.h"

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

void Framebuffer::SetPixelColour(const Colour& colour, const Point& hit_point) {
    assert(hit_point.x < m_width && "p.x must be in range of framebuffer width");
    assert(hit_point.y < m_height && "p.y must be in range of framebuffer height");

    // scale unit value (0.0 - 0.1) to rgb (0 - 256)
    static const Interval intensity(0.000, 0.999);
    Colour scaled_colour{ intensity.Clamp(colour.x()) * 256
                        , intensity.Clamp(colour.y()) * 256
                        , intensity.Clamp(colour.z()) * 256 };

    const int row_offset{ hit_point.y * m_width };
    pixels[row_offset + hit_point.x] = scaled_colour;
}
