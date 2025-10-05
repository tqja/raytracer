#pragma once

#include "Utility.h"

class Interval {
public:
    Interval() : m_min(+infinity), m_max(-infinity) {} // empty interval
    Interval(double min, double max) : m_min(min), m_max(max) {}

    bool Contains(double x) const {
        return m_min <= x && x <= m_max;
    }

    bool Surrounds(double x) const {
        return m_min < x && x < m_max;
    }

    double Clamp(double x) const {
        if (x < m_min) return m_min;
        if (x > m_max) return m_max;
        return x;
    }

    double Min() const {
        return m_min;
    }

    double Max() const {
        return m_max;
    }

    double Size() const {
        return m_max - m_min;
    }

    static Interval Empty() {
        return Interval(+infinity, -infinity);
    }

    static Interval Universe() {
        return Interval(-infinity, +infinity);
    }

private:
    double m_min{};
    double m_max{};
};
