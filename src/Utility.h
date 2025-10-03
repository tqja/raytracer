#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


using std::make_shared;
using std::shared_ptr;

constexpr double infinity{ std::numeric_limits<double>::infinity() };
constexpr double pi{ 3.1415926535897932385 };

inline double DegToRads(double degrees) {
    return degrees * pi / 180.0;
}
