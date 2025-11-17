#include "Framebuffer.h"
#include "Colour.h"
#include "Vec3.h"
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

    pixels.resize(static_cast<uint64_t>(m_width) * static_cast<uint64_t>(m_height));
}

static float LinearToGamma(float linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

void Framebuffer::SetPixelColour(const Colour& colour, const Point3& hit_point) {
    assert(hit_point.x < m_width && "p.x must be in range of framebuffer width");
    assert(hit_point.y < m_height && "p.y must be in range of framebuffer height");

    float r = LinearToGamma(colour.x());
    float g = LinearToGamma(colour.y());
    float b = LinearToGamma(colour.z());

    // scale unit value (0.0 - 1.0) to rgb (0 - 256)
    static const Interval intensity(0.000f, 0.999f);
    Colour scaled_colour{ intensity.Clamp(r) * 256
                        , intensity.Clamp(g) * 256
                        , intensity.Clamp(b) * 256 };

    const int row_offset{ static_cast<int>(hit_point.y()) * m_width };
     pixels[row_offset + static_cast<int>(hit_point.x())] = scaled_colour;
}