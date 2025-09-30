#pragma once
#include "Ray.h"

struct Point {
    int x{};
    int y{};
};

class Sphere {
public:
    Sphere() {};
    Sphere(Vec3 center, double radius)
        : m_center{ center }, m_radius{ radius } {};

    bool hit(const Ray& ray) const;

private:
    Vec3 m_center{};
    double m_radius{};
};