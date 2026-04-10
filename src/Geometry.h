#pragma once

#include <cmath>
#include <memory>

#include "Hittable.h"
#include "Interval.h"
#include "Material.h"
#include "Ray.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
    Sphere() {};
    Sphere(Point3 center, float radius, shared_ptr<Material> material)
        : m_center{ center }, m_radius{ std::fmaxf(0, radius) }, m_material(material) {};

    void hit(const RayGroup& rays, const IntervalGroup& ray_t, HitRecordGroup& records) const override;

    const Point3& GetCenter() const { return m_center; }
    const float& GetRadius() const { return m_radius; }

private:
    Vec3 m_center{};
    float m_radius{};
    shared_ptr<Material> m_material;
};