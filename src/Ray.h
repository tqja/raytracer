#pragma once

#include <vector>

#include "Vec3.h"

class RayGroup {
public:
    RayGroup() {};
    RayGroup(const Vec3Group& origins, const Vec3Group& directions) : m_origins(origins), m_directions(directions) {};

    const size_t Size() const { return m_origins.Size(); }

    const Vec3Group GetOrigins() const { return m_origins; }

    const Vec3Group GetDirections() const { return m_directions; }

    void SetOrigins(const Vec3Group& origins) { m_origins = origins; }

    void SetDirections(const Vec3Group& directions) { m_directions = directions; }

    void SetDirectionAtIndex(size_t i, const Vec3& direction) { m_directions.SetElement(i, direction); }

    void SetOriginAtIndex(size_t i, const Vec3& origin) { m_origins.SetElement(i, origin); }

    void ReplaceDirections(const Vec3Group& replace, const std::vector<float>& mask) {
        m_directions.BlendVectors(replace, mask);
    }

    void ReplaceOrigins(const Vec3Group& replace, const std::vector<float>& mask) {
        m_origins.BlendVectors(replace, mask);
    }

    Vec3Group at(const std::vector<float>& t) const {
        Vec3Group result{};
        m_directions.MulAdd(t, m_origins, result);
        return result;
    }

private:
    Vec3Group m_origins;
    Vec3Group m_directions;
};

class Ray {
public:
    Ray() {};
    Ray(const Point3& origin, const Vec3& direction) : m_origin{ origin }, m_direction{ direction } {};

    const Point3& GetOrigin() const { return m_origin; }
    const Vec3& GetDirection() const { return m_direction; }

    const Point3 at(float t) const { return m_origin + t * m_direction; }

private:
    Point3 m_origin{};
    Vec3 m_direction{};
};
