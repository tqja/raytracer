#include "Geometry.h"

bool Sphere::hit(const Ray& ray) const {
    Vec3 oc = m_center - ray.GetOrigin();

    double a = dot(ray.GetDirection(), ray.GetDirection());
    double b = -2.0 * dot(ray.GetDirection(), oc);
    double c = dot(oc, oc) - pow(m_radius, 2);

    double discriminant = pow(b, 2) - (4.0 * a * c);
    return discriminant >= 0;
}