#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include "Utility.h"

class Vec3Group {
public:
    Vec3Group() : m_x(globals::samples), m_y(globals::samples), m_z(globals::samples) {};

    std::vector<float> x() { return m_x; }
    std::vector<float> y() { return m_y; }
    std::vector<float> z() { return m_z; }

    void ZeroX() { std::memset(m_x.data(), 0, sizeof(m_x)); }
    void ZeroY() { std::memset(m_y.data(), 0, sizeof(m_y)); }
    void ZeroZ() { std::memset(m_z.data(), 0, sizeof(m_z)); }
    void Zero() { 
        ZeroX();
        ZeroY();
        ZeroZ();
    }

private:
    std::vector<float> m_x;
    std::vector<float> m_y;
    std::vector<float> m_z;
};

class Vec3 {
public:
    float e[3];

    Vec3() : e{ 0,0,0 } {}
    Vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(float t) {
        return *this *= 1 / t;
    }

    float Length() const {
        return std::sqrt(LengthSquared());
    }

    float LengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool NearZero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static Vec3 Random() {
        return Vec3(RandomFloat(), RandomFloat(), RandomFloat());
    }

    static Vec3 Random(float min, float max) {
        return Vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
    }


};

using Point3 = Vec3;


inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline bool operator==(const Vec3& a, const Vec3& b) {
    constexpr float EPS = 1e-6f;
    return std::fabs(a[0] - b[0]) <= EPS
        && std::fabs(a[1] - b[1]) <= EPS
        && std::fabs(a[2] - b[2]) <= EPS;
}

inline bool operator!=(const Vec3& a, const Vec3& b) {
    return !(a == b);
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(float t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, float t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, float t) {
    return (1 / t) * v;
}

inline float Dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 UnitVector(const Vec3& v) {
    return v / v.Length();
}

inline Vec3 RandomUnitVector() {
    while (true) {
        Vec3 hit_point{ Vec3::Random(-1, 1) };
        float LengthSquared{ hit_point.LengthSquared() };

        if (1e-160 < LengthSquared && LengthSquared <= 1) {
            return hit_point / sqrt(LengthSquared);
        }
    }
}

inline Vec3 RandomOnHemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere{ RandomUnitVector() };
    bool in_same_hemisphere{ Dot(on_unit_sphere, normal) > 0.0f };

    if (in_same_hemisphere) {
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& normal) {
    return v - 2 * Dot(v, normal) * normal;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
    float cos_theta = std::fminf(Dot(-uv, n), 1.0f);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrtf(std::fabs(1.0f - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 RandomInUnitDisk() {
    while (true) {
        auto p = Vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0);
        if (p.LengthSquared() < 1)
            return p;
    }
}
