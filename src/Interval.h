#pragma once

#include <cassert>
#include <vector>

#include "Utility.h"
#include "simd/simd.h"

class Interval {
public:
    Interval() : m_min(+infinity), m_max(-infinity) {}  // empty interval
    Interval(float min, float max) : m_min(min), m_max(max) {}

    bool Contains(float x) const { return m_min <= x && x <= m_max; }

    bool Surrounds(float x) const { return m_min < x && x < m_max; }

    void Contains(const float* x, float* result) const {
        simd::DispatchBase* dp{ simd::GetDispatch() };
        float* lower{};
        dp->GreaterEqual(x, m_min, lower, 1);

        float* upper{};
        dp->LessEqual(x, m_max, upper, 1);

        dp->And(lower, upper, result, globals::samples);
    }

    void Surrounds(const float* x, float* result) const {
        simd::DispatchBase* dp{ simd::GetDispatch() };
        std::vector<float> lower(globals::samples);
        dp->GreaterThan(x, m_min, lower.data(), 1);

        std::vector<float> upper(globals::samples);
        dp->LessThan(x, m_max, upper.data(), 1);

        dp->And(lower.data(), upper.data(), result, globals::samples);
    }

    float Clamp(float x) const {
        if (x < m_min) return m_min;
        if (x > m_max) return m_max;
        return x;
    }

    float Min() const { return m_min; }

    float Max() const { return m_max; }

    size_t Size() const { return static_cast<size_t>(m_max - m_min); }

    static Interval Empty() { return Interval(+infinity, -infinity); }

    static Interval Universe() { return Interval(-infinity, +infinity); }

private:
    float m_min{};
    float m_max{};
};

class IntervalGroup {
public:
    IntervalGroup() { m_size = 0; }

    IntervalGroup(const float min, const float max, const size_t size) {
        m_min = std::vector<float>(size, min);
        m_max = std::vector<float>(size, max);
        m_size = size;
    }

    IntervalGroup(const std::vector<float>& min_vals, const std::vector<float>& max_vals)
        : m_min{ min_vals }, m_max{ max_vals } {
        m_size = m_min.size();
    }

    const std::vector<float> Min() const { return m_min; }

    const std::vector<float> Max() const { return m_max; }

    void Contains(const float* vals, float* result) const {
        simd::DispatchBase* dp{ simd::GetDispatch() };
        std::vector<float> lower(m_size);
        dp->GreaterEqual(vals, m_min.data(), lower.data(), m_size);
        std::vector<float> upper(m_size);
        dp->LessEqual(vals, m_max.data(), upper.data(), m_size);
        dp->And(lower.data(), upper.data(), result, m_size);
    }

    void Surrounds(const float* vals, float* result) const {
        simd::DispatchBase* dp{ simd::GetDispatch() };
        std::vector<float> lower(m_size);
        dp->GreaterThan(vals, m_min.data(), lower.data(), m_size);
        std::vector<float> upper(m_size);
        dp->LessThan(vals, m_max.data(), upper.data(), m_size);
        dp->And(lower.data(), upper.data(), result, m_size);
    }

    const std::vector<float> Clamp(const std::vector<float>& vals) const {
        assert(vals.size() == m_size);
        simd::DispatchBase* dp{ simd::GetDispatch() };
        std::vector<float> clamped_values(m_size);
        dp->Max(vals.data(), m_min.data(), clamped_values.data(), m_size);
        dp->Min(clamped_values.data(), m_max.data(), clamped_values.data(), m_size);

        return clamped_values;
    }

    void SetMin(const std::vector<float>& min_vals) {
        assert(m_max.size() == min_vals.size());
        m_min = min_vals;
    }

    void SetMax(const std::vector<float>& max_vals) {
        assert(m_min.size() == max_vals.size());
        m_max = max_vals;
    }

    void SetInterval(const std::vector<float>& min_vals, const std::vector<float>& max_vals) {
        assert(min_vals.size() == max_vals.size());
        m_min = min_vals;
        m_max = max_vals;
    }

    static IntervalGroup Universe(size_t size) {
        return IntervalGroup(std::vector<float>(size, -infinity), std::vector<float>(size, +infinity));
    }

    static IntervalGroup Empty(size_t size) {
        return IntervalGroup(std::vector<float>(size, +infinity), std::vector<float>(size, -infinity));
    }

    void Size(std::vector<float>& out) const {
        simd::DispatchBase* dp = simd::GetDispatch();
        dp->Sub(m_max.data(), m_min.data(), out.data(), m_size);
    }

private:
    std::vector<float> m_min{};
    std::vector<float> m_max{};
    size_t m_size{};
};
