#include "Geometry.h"
#include <cmath>

double Sphere::hit(const Ray& ray) const {
    constexpr double no_hit{ -1.0 };
    Vec3 oc = m_center - ray.GetOrigin();
    Vec3 dir = ray.GetDirection();

    // h == b/-2 allowing a simplification of the quadratic
    double a{ dir.length_squared() };
    double h{ dot(dir, oc) };
    double c{ oc.length_squared() - m_radius * m_radius };
    double discriminant{ h * h - a * c };

    if (discriminant < 0) {
        return no_hit;
    }

    return (h - std::sqrt(discriminant)) / a;
}