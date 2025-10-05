#pragma once

#include "Interval.h"
#include "Ray.h"

class HitRecord {
public:
    Point3 p{};
    Vec3 normal{};
    double t{};
    bool front_face{};

    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        front_face = dot(ray.GetDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : - outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray&, Interval ray_t, HitRecord& rec) const = 0;
};