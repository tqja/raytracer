#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Ray.h"


struct Point {
    int x{};
    int y{};
};

class Sphere : public Hittable {
public:
    Sphere() {};
    Sphere(Point3 center, double radius, shared_ptr<Material> material)
        : m_center{ center },
          m_radius{ std::fmax(0, radius) },
          m_material(material) {
    };

    bool hit(const Ray&, const Interval& ray_t, HitRecord& record) const override;

    const Point3& GetCenter() const { return m_center; }
    const double& GetRadius() const { return m_radius; }

private:
    Vec3 m_center{};
    double m_radius{};
    shared_ptr<Material> m_material;
};