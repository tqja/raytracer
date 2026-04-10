#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

#include "Utility.h"
#include "simd/simd.h"

class Vec3;
class Vec3Group;

template <typename T>
concept VecLike = std::is_same_v<Vec3, T> || std::is_same_v<Vec3Group, T>;

class Vec3 {
public:
    float e[3];

    Vec3() : e{ 0, 0, 0 } {}
    Vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

    const float x() const { return e[0]; }
    const float y() const { return e[1]; }
    const float z() const { return e[2]; }

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

    Vec3& operator/=(float t) { return *this *= 1 / t; }

    float Length() const { return std::sqrt(LengthSquared()); }

    float LengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    bool NearZero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static Vec3 Random() { return Vec3(RandomFloat(), RandomFloat(), RandomFloat()); }

    static Vec3 Random(float min, float max) {
        return Vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
    }

    void Broadcast(float value) {
        e[0] = value;
        e[1] = value;
        e[2] = value;
    }
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline bool operator==(const Vec3& a, const Vec3& b) {
    constexpr float EPS = 1e-6f;
    return std::fabs(a[0] - b[0]) <= EPS && std::fabs(a[1] - b[1]) <= EPS && std::fabs(a[2] - b[2]) <= EPS;
}

inline bool operator!=(const Vec3& a, const Vec3& b) { return !(a == b); }

inline Vec3 operator+(const Vec3& u, const Vec3& v) { return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); }

inline Vec3 operator-(const Vec3& u, const Vec3& v) { return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); }

inline Vec3 operator*(const Vec3& u, const Vec3& v) { return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]); }

inline Vec3 operator*(float t, const Vec3& v) { return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }

inline Vec3 operator*(const Vec3& v, float t) { return t * v; }

inline Vec3 operator/(const Vec3& v, float t) { return (1 / t) * v; }

inline float Dot(const Vec3& u, const Vec3& v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 UnitVector(const Vec3& v) { return v / v.Length(); }

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

inline Vec3 Reflect(const Vec3& v, const Vec3& normal) { return v - 2 * Dot(v, normal) * normal; }

inline Vec3 Refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
    float cos_theta = std::fminf(Dot(-uv, n), 1.0f);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrtf(std::fabs(1.0f - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 RandomInUnitDisk() {
    while (true) {
        auto p = Vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0);
        if (p.LengthSquared() < 1) return p;
    }
}

class Vec3Group {
public:
    Vec3Group() : m_x(globals::samples), m_y(globals::samples), m_z(globals::samples), m_size{ globals::samples } {};
    Vec3Group(std::vector<float> x, std::vector<float> y, std::vector<float> z) : m_x{ x }, m_y{ y }, m_z{ z } {
        m_size = m_x.size();
        m_temp_buffer.resize(m_size);
    }

    std::vector<float>& x() { return m_x; }
    std::vector<float>& y() { return m_y; }
    std::vector<float>& z() { return m_z; }

    const std::vector<float>& x() const { return m_x; }
    const std::vector<float>& y() const { return m_y; }
    const std::vector<float>& z() const { return m_z; }

    void ZeroX() { std::memset(m_x.data(), 0, sizeof(m_x)); }
    void ZeroY() { std::memset(m_y.data(), 0, sizeof(m_y)); }
    void ZeroZ() { std::memset(m_z.data(), 0, sizeof(m_z)); }
    void Zero() {
        ZeroX();
        ZeroY();
        ZeroZ();
    }

    void SetElement(size_t i, const Vec3& vec) {
        m_x[i] = vec.x();
        m_y[i] = vec.y();
        m_z[i] = vec.z();
    }

    const size_t Size() const { return m_size; }

    template <typename Function>
    void Generate(Function&& func) {
        for (size_t i = 0; i < m_size; i++) {
            Vec3 vec = func();
            SetElement(i, vec);
        }
    }

    template <typename Function>
    void MaskedGenerate(Function&& func, const std::vector<float>& mask) {
        for (size_t i = 0; i < m_size; i++) {
            if (!mask[i]) continue;
            Vec3 vec = func();
            SetElement(i, vec);
        }
    }

    void SetXAtIndex(size_t i, const float value) { m_x[i] = value; }

    void SetYAtIndex(size_t i, const float value) { m_y[i] = value; }

    void SetZAtIndex(size_t i, const float value) { m_z[i] = value; }

    void Broadcast(Vec3 vec) {
        std::fill(m_x.begin(), m_x.end(), vec.x());
        std::fill(m_y.begin(), m_y.end(), vec.y());
        std::fill(m_z.begin(), m_z.end(), vec.z());
    }

    template <VecLike V>
    void Add(const V& other) {
        BinaryOperation(other, [this]<typename FloatLike>(float* inout, const FloatLike in, const size_t num) {
            m_dp->Add(inout, in, num);
        });
    }

    template <VecLike V>
    void Add(const V& other, Vec3Group& out) const {
        BinaryOperation(other, out,
                        [this]<typename FloatLike>(const float* l, const FloatLike r, float* out, const size_t num) {
                            m_dp->Add(l, r, out, num);
                        });
    }

    template <VecLike V>
    void Sub(const V& other) {
        BinaryOperation(other, [this]<typename FloatLike>(float* inout, const FloatLike in, const size_t num) {
            m_dp->Sub(inout, in, num);
        });
    }

    template <VecLike V>
    void Sub(const V& other, Vec3Group& out) const {
        BinaryOperation(other, out,
                        [this]<typename FloatLike>(const float* l, const FloatLike r, float* out, const size_t num) {
                            m_dp->Sub(l, r, out, num);
                        });
    }

    template <VecLike VecLikeA, VecLike VecLikeB>
    void SubVecLike(const VecLikeA& a, const VecLikeB& b) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Sub(GetOperand(a, axis), GetOperand(b, axis), GetOperand(*this, axis), globals::samples);
        }
    }

    template <VecLike V>
    void Mul(const V& other) {
        BinaryOperation(
            other, [this]<typename FloatLike>(float* l, const FloatLike r, const size_t num) { m_dp->Mul(l, r, num); });
    }

    template <VecLike V>
    void Mul(const V& other, Vec3Group& out) const {
        BinaryOperation(other, out,
                        [this]<typename FloatLike>(const float* l, const FloatLike r, float* out, const size_t num) {
                            m_dp->Mul(l, r, out, num);
                        });
    }

    void Mul(const std::vector<float>& other, Vec3Group& out) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(GetOperand(*this, axis), other.data(), GetOperand(out, axis), m_size);
        }
    }

    void MulScalar(float other) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(GetOperand(*this, axis), other, m_size);
        }
    }

    void MulScalar(const float other, Vec3Group& out) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(GetOperand(*this, axis), other, GetOperand(out, axis), m_size);
        }
    }

    /*template <VecLike V>
    void MulAxis(const V& other) const {
        BinaryOperation(other, out,
                        [this]<typename FloatLike>(const float* l, const FloatLike r, float* out, const size_t num) {
                            m_dp->Mul(l, r, out, num);
                        });
    }

    void MulAxis(float* other, int axis) { m_dp->Mul(GetOperand(*this, axis), other, m_size); }*/

    template <VecLike VecLikeB, VecLike VecLikeC>
    void MulAdd(const VecLikeB& b, const VecLikeC& c) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->MulAdd(GetOperand(*this, axis), GetOperand(b, axis), GetOperand(c, axis), m_size);
        }
    }

    template <VecLike VecLikeC>
    void MulAdd(const std::vector<float>& b, const VecLikeC& c) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->MulAdd(GetOperand(*this, axis), b.data(), GetOperand(c, axis), m_size);
        }
    }

    template <VecLike VecLikeC>
    void MulAdd(const std::vector<float>& b, const VecLikeC& c, Vec3Group& out) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->MulAdd(GetOperand(*this, axis), b.data(), GetOperand(c, axis), GetOperand(out, axis), m_size);
        }
    }

    // takes value from this object if mask is true, else takes value from replace
    void BlendVectors(const Vec3Group& replace, const std::vector<float>& mask) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->BlendVectors(mask.data(), GetOperand(*this, axis), GetOperand(replace, axis), globals::samples);
        }
    }

    void BlendScalar(const float replace, const std::vector<float>& mask) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->BlendVectorScalar(mask.data(), GetOperand(*this, axis), replace, m_size);
        }
    }

    void Reflect(const Vec3Group& normals, Vec3Group& out) const {
        m_temp_buffer.resize(m_size);
        std::vector<float> dot_products(normals.Size());
        Dot(normals, dot_products);
        m_dp->Mul(dot_products.data(), 2.0f, globals::samples);

        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(dot_products.data(), GetOperand(normals, axis), m_temp_buffer.data(), globals::samples);
            m_dp->Sub(GetOperand(*this, axis), m_temp_buffer.data(), GetOperand(out, axis), globals::samples);
        }
    }

    void Refract(const Vec3Group& uv, const Vec3Group& n, std::vector<float> etai_over_etat, Vec3Group& out) {
        Vec3Group temp{};

        // cos_theta = Min(Dot(-uv, n), 1.0f)
        uv.Neg(temp);
        std::vector<float> dot_products{};
        temp.Dot(n, dot_products);

        std::vector<float> cos_theta(globals::samples);
        m_dp->Min(dot_products.data(), 1.0f, cos_theta.data(), globals::samples);

        // r_out_perp = etai_over_etat * (uv + cos_theta * n)
        Vec3Group r_out_perp{};
        n.MulAdd(cos_theta, uv, r_out_perp);

        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(GetOperand(r_out_perp, axis), etai_over_etat.data(), GetOperand(r_out_perp, axis),
                      globals::samples);
        }

        // r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.LengthSquared())) * n
        std::vector<float> r_out_perp_length_squared(globals::samples);
        r_out_perp.LengthSquared(r_out_perp_length_squared.data());
        std::vector<float> temp_vec(globals::samples);
        m_dp->Sub(1.0f, r_out_perp_length_squared.data(), temp_vec.data(), globals::samples);
        m_dp->Abs(temp_vec.data(), globals::samples);
        m_dp->Sqrt(temp_vec.data(), globals::samples);
        m_dp->Neg(temp_vec.data(), globals::samples);

        Vec3Group r_out_parallel{};
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Mul(temp_vec.data(), GetOperand(n, axis), GetOperand(r_out_parallel, axis), globals::samples);
        }

        r_out_perp.Add(r_out_parallel, out);
    }

    void Squared(Vec3Group& out) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Squared(GetOperand(*this, axis), GetOperand(out, axis), globals::samples);
        }
    }

    void Neg(Vec3Group& out) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Neg(GetOperand(*this, axis), GetOperand(out, axis), globals::samples);
        }
    }

    void LengthSquared(float* out) const {
        m_temp_buffer.resize(m_size);
        m_dp->Squared(GetOperand(*this, Axis::ax), out, globals::samples);
        m_dp->MulAdd(GetOperand(*this, Axis::ay), GetOperand(*this, Axis::ay), out, m_temp_buffer.data(),
                     globals::samples);
        m_dp->MulAdd(GetOperand(*this, Axis::az), GetOperand(*this, Axis::az), m_temp_buffer.data(), out,
                     globals::samples);
    }

    void Length(float* out) const {
        std::vector<float> length_squared(globals::samples);
        LengthSquared(length_squared.data());
        m_dp->Sqrt(length_squared.data(), out, globals::samples);
    }

    Vec3 Reduce() {
        Vec3 sum{};

        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            sum.e[axis] = m_dp->ReduceSum(GetOperand(*this, axis), globals::samples);
        }

        return sum;
    }

    void UnitVectors() {
        std::vector<float> length(globals::samples);
        Length(length.data());

        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Div(GetOperand(*this, axis), length.data(), globals::samples);
        }
    }

    void UnitVectors(Vec3Group& out) const {
        std::vector<float> length(globals::samples);
        Length(length.data());

        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->Div(GetOperand(*this, axis), length.data(), GetOperand(out, axis), globals::samples);
        }
    }

    void Dot(const Vec3Group& other, std::vector<float>& out) const {
        out.resize(m_size);
        m_temp_buffer.resize(m_size);

        // out = this.x * other.x
        std::copy(m_x.begin(), m_x.end(), out.begin());
        m_dp->Mul(out.data(), GetOperand(other, Axis::ax), m_size);

        // temp = this.y * other.y + out
        std::copy(m_y.begin(), m_y.end(), m_temp_buffer.begin());
        m_dp->MulAdd(m_temp_buffer.data(), GetOperand(other, Axis::ay), out.data(), m_size);

        // out = this.z * other.z + temp (final dot product)
        std::copy(m_z.begin(), m_z.end(), out.begin());
        m_dp->MulAdd(out.data(), GetOperand(other, Axis::az), m_temp_buffer.data(), m_size);
    }

    std::vector<float> NearZero() const {
        constexpr float s = 1e-8f;
        simd::DispatchBase* dp{ simd::GetDispatch() };
        std::vector<float> result(globals::samples);
        std::vector<float> abs_values(globals::samples);
        std::vector<float> mask(globals::samples);

        dp->Abs(m_x.data(), abs_values.data(), globals::samples);
        dp->LessThan(abs_values.data(), s, result.data(), globals::samples);

        dp->Abs(m_y.data(), abs_values.data(), globals::samples);
        dp->LessThan(abs_values.data(), s, mask.data(), globals::samples);
        dp->And(result.data(), mask.data(), result.data(), globals::samples);

        dp->Abs(m_z.data(), abs_values.data(), globals::samples);
        dp->LessThan(abs_values.data(), s, mask.data(), globals::samples);
        dp->And(result.data(), mask.data(), result.data(), globals::samples);

        return result;
    }

    void BlendVectors(const Vec3Group& replace, const std::vector<float>& mask, Vec3Group& out) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            m_dp->BlendVectors(GetOperand(*this, axis), GetOperand(replace, axis), mask.data(), GetOperand(out, axis),
                               globals::samples);
        }
    }

    Vec3 operator[](size_t i) const { return Vec3{ m_x[i], m_y[i], m_z[i] }; }

    template <VecLike V>
    void operator+=(const V& other) {
        Add(other);
    }

    template <VecLike V>
    void operator-=(const V& other) {
        Sub(other, *this);
    }

    template <VecLike V>
    void operator*=(const V& other) {
        Sub(other, *this);
    }

private:
    std::vector<float> m_x{};
    std::vector<float> m_y{};
    std::vector<float> m_z{};
    size_t m_size{};
    mutable std::vector<float> m_temp_buffer{};

    enum Axis { ax, ay, az };

    simd::DispatchBase* m_dp{ simd::GetDispatch() };

    template <VecLike V, typename Operation>
    void BinaryOperation(const V& other, Operation operation) {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            operation(GetOperand(*this, axis), GetOperand(other, axis), m_size);
        }
    }

    template <VecLike V, typename Operation>
    void BinaryOperation(const V& other, Vec3Group& out, Operation operation) const {
        for (size_t axis = Axis::ax; axis <= Axis::az; axis++) {
            operation(GetOperand(*this, axis), GetOperand(other, axis), GetOperand(out, axis), m_size);
        }
    }

    template <VecLike V>
    static auto GetOperand(V& v, size_t axis) {
        if constexpr (std::is_same_v<std::remove_reference_t<V>, Vec3>) {
            if (axis == Axis::ax) return v.x();
            if (axis == Axis::ay) return v.y();
            return v.z();
        } else {
            if (axis == Axis::ax) return v.x().data();
            if (axis == Axis::ay) return v.y().data();
            return v.z().data();
        }
    }

    template <VecLike V>
    static auto GetOperand(const V& v, size_t axis) {
        if constexpr (std::is_same_v<std::remove_reference_t<V>, Vec3>) {
            if (axis == Axis::ax) return v.x();
            if (axis == Axis::ay) return v.y();
            return v.z();
        } else {
            if (axis == Axis::ax) return v.x().data();
            if (axis == Axis::ay) return v.y().data();
            return v.z().data();
        }
    }
};
