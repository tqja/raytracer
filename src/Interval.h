#pragma once

#include "Utility.h"

class Interval {
public:
    Interval() : m_min(+infinity), m_max(-infinity) {} // empty interval
    Interval(float min, float max) : m_min(min), m_max(max) {}

    bool Contains(float x) const {
        return m_min <= x && x <= m_max;
    }

    bool Surrounds(float x) const {
        return m_min < x && x < m_max;
    }

    float Clamp(float x) const {
        if (x < m_min) return m_min;
        if (x > m_max) return m_max;
        return x;
    }

    float Min() const {
        return m_min;
    }

    float Max() const {
        return m_max;
    }

    float Size() const {
        return m_max - m_min;
    }

    static Interval Empty() {
        return Interval(+infinity, -infinity);
    }

    static Interval Universe() {
        return Interval(-infinity, +infinity);
    }

private:
    float m_min{};
    float m_max{};
};
