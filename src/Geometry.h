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

    double hit(const Ray& ray) const;

    const Vec3& GetCenter() const { return m_center; }
    const double& GetRadius() const { return m_radius; }

private:
    Vec3 m_center{};
    double m_radius{};
};