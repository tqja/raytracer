#pragma once

#include "Colour.h"
#include "Geometry.h"
#include <vector>

class Framebuffer {
public:
    static constexpr int max_width{ 8192 };
    static constexpr int max_height{ 8192 };

    Framebuffer(int width, int height);

    const std::vector<Colour>& GetPixels() const { return pixels; }
    void SetPixelColour(const Colour& colour, const Point& hit_point);

private:
    int m_width{};
    int m_height{};
    std::vector<Colour> pixels{};
};

