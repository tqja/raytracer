#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Ray.h"

class Sphere : public Hittable {
public:
    Sphere() {};
    Sphere(Point3 center, float radius, shared_ptr<Material> material)
        : m_center{ center },
          m_radius{ std::fmaxf(0, radius) },
          m_material(material) {
    };

    bool hit(const Ray&, const Interval& ray_t, HitRecord& record) const override;

    const Point3& GetCenter() const { return m_center; }
    const float& GetRadius() const { return m_radius; }

private:
    Vec3 m_center{};
    float m_radius{};
    shared_ptr<Material> m_material;
};