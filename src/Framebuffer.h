#pragma once

#include "colour.h"
#include "Point.h"
#include <vector>

class Framebuffer {
private:
    int height{};
    int width{};
    std::vector<colour> pixels;
    void set_pixel_colour(const colour pixel_colour, const Point p);

public:
    Framebuffer(int height, int width);
    void init_default();
    std::vector<colour> get_framebuffer() { return pixels; }
};

