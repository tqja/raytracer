#include "Geometry.h"
#include <cmath>

bool Sphere::hit(const Ray& ray, const Interval& ray_t, HitRecord& record) const {
    Vec3 oc = m_center - ray.GetOrigin();
    Vec3 dir = ray.GetDirection();

    // h == b/-2 allowing a simplification of the quadratic
    double a{ dir.LengthSquared() };
    double h{ Dot(dir, oc) };
    double c{ oc.LengthSquared() - m_radius * m_radius };

    double discriminant{ h * h - a * c };
    if (discriminant < 0) {
        return false;
    }

    double sqrt_d{ std::sqrt(discriminant) };

    double root = (h - sqrt_d) / a;
    if (!ray_t.Surrounds(root)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.Surrounds(root)) {
            return false;
        }
    }

    record.t = root;
    record.hit_point = ray.at(record.t);
    Vec3 outward_normal = (record.hit_point - m_center) / m_radius;
    record.SetFaceNormal(ray, outward_normal);
    record.material = m_material;

    return true;
}