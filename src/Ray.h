#pragma once

#include "Vec3.h"

class Ray
{
public:
    Ray() {};
    Ray(const Point3& GetOrigin, const Vec3& GetDirection) : orig(GetOrigin), dir(GetDirection) {};

    const Point3& GetOrigin() const { return orig; }
    const Vec3& GetDirection() const { return dir; }

    Point3 at(double t) const {
        return orig + t * dir;
    }

private:
    Point3 orig;
    Vec3 dir;
};

