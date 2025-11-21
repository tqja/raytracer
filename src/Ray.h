#pragma once

#include "Vec3.h"

class RayGroup {
public:
    RayGroup() {};
    RayGroup(const Vec3Group& origins, const Vec3Group& directions) : m_origins(origins), m_directions(directions) {};

    const Vec3Group GetOrigins() const {
        return m_origins;
    }
    const Vec3Group GetDirections() const {
        return m_directions;
    }
private:
    Vec3Group m_origins;
    Vec3Group m_directions;
};




class Ray
{
public:
    Ray() {};
    Ray(const Point3& origin, const Vec3& direction)
        : m_origin{ origin }, m_direction{ direction } {};

    const Point3& GetOrigin() const { return m_origin; }
    const Vec3& GetDirection() const { return m_direction; }

    const Point3 at(float t) const {
        return m_origin + t * m_direction;
    }

private:
    Point3 m_origin{};
    Vec3 m_direction{};
};

