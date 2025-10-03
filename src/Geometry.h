#pragma once

#include "Hittable.h"
#include "Ray.h"


struct Point {
    int x{};
    int y{};
};

class Sphere : public Hittable {
public:
    Sphere() {};
    Sphere(Point3 center, double radius)
        : m_center{ center }, m_radius{ std::fmax(0, radius) } {
    };

    bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& record) const override;

    const Point3& GetCenter() const { return m_center; }
    const double& GetRadius() const { return m_radius; }

private:
    Vec3 m_center{};
    double m_radius{};
};