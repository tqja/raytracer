#pragma once

#include "colour.h"
#include "Point.h"
#include <vector>

class Framebuffer {
public:
    static constexpr int max_width{ 8192 };
    static constexpr int max_height{ 8192 };

    Framebuffer(int width, int height);

    std::vector<colour> GetFramebuffer() { return pixels; }
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    void SetPixelColour(const colour pixel_colour, const Point p);
    void InitDefault();


private:
    int width{};
    int height{};
    std::vector<colour> pixels;
};

