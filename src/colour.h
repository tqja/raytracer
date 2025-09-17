#pragma once

#include <vector>
#include "vec3.h"

#include <cassert>

class Colour : public vec3 {
public:
    Colour() : vec3(0, 0, 0) {}
    Colour(double r, double g, double b) : vec3(r, g, b) {
        assert(0.0 <= x() && x() < 256.0 && "red component must be in range [0.0, 256.0)");
        assert(0.0 <= y() && y() < 256.0 && "green component must be in range [0.0, 256.0)");
        assert(0.0 <= z() && z() < 256.0 && "blue component must be in range [0.0, 256.0)");
    }
};


