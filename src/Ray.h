#pragma once

#include "Vec3.h"

struct RayGroup {
    Vec3Group origin;
    Vec3Group direction;
};




class Ray
{
public:
    Ray() {};
    Ray(const Point3& origin, const Vec3& direction)
        : m_origin{ origin }, m_direction{ direction } {};

    const Point3& GetOrigin() const { return m_origin; }
    const Vec3& GetDirection() const { return m_direction; }

    Point3 at(float t) const {
        return m_origin + t * m_direction;
    }

private:
    Point3 m_origin{};
    Vec3 m_direction{};
};

