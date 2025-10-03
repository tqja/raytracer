#include "Geometry.h"
#include <cmath>

bool Sphere::hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& record) const {
    Vec3 oc = m_center - ray.GetOrigin();
    Vec3 dir = ray.GetDirection();

    // h == b/-2 allowing a simplification of the quadratic
    double a{ dir.length_squared() };
    double h{ dot(dir, oc) };
    double c{ oc.length_squared() - m_radius * m_radius };

    double discriminant{ h * h - a * c };
    if (discriminant < 0) {
        return false;
    }

    double sqrt_d{ std::sqrt(discriminant) };
    auto in_range = [ray_tmin, ray_tmax](double x) {
        return ray_tmin < x && x < ray_tmax;
    };

    double root = (h - sqrt_d) / a;
    if (!in_range(root)) {
        root = (h + sqrt_d) / a;
        if (!in_range(root)) {
            return false;
        }
    }

    record.t = root;
    record.p = ray.at(record.t);
    Vec3 outward_normal = (record.p - m_center) / m_radius;
    record.SetFaceNormal(ray, outward_normal);

    return true;
}