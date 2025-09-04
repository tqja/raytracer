#pragma once

#include <vector>
#include "vec3.h"

using colour = vec3;

void write_pixel_to_framebuffer(
    std::vector<vec3>& framebuffer,
    const colour&      pixel_colour,
    const int          x,
    const int          y,
    const int          width
) {
    const int row_offset = y * width;
    framebuffer[row_offset + x] = pixel_colour;
}

