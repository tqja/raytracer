#include "Framebuffer.h"
#include "Colour.h"
#include "Geometry.h"

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

void Framebuffer::SetPixelColour(const Colour& colour, const Point& p) {
    assert(p.x < m_width && "p.x must be in range of framebuffer width");
    assert(p.y < m_height && "p.y must be in range of framebuffer height");

    // scale unit value (0.0 - 0.1) to rgb (0 - 256)
    Colour scaled_colour{ colour.x() * 255.999
                        , colour.y() * 255.999
                        , colour.z() * 255.999 };

    const int row_offset{ p.y * m_width };
    pixels[row_offset + p.x] = scaled_colour;
}
