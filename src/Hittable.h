#pragma once

#include "Interval.h"
#include "Ray.h"

class Material;

class HitRecord {
public:
    Point3 hit_point{};
    Vec3 normal{};
    shared_ptr<Material> material;
    double t{};
    bool front_face{};

    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        front_face = Dot(ray.GetDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : - outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray&, const Interval& ray_t, HitRecord& rec) const = 0;
};