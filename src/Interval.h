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

    double GetMin() const {
        return m_min;
    }

    double GetMax() const {
        return m_max;
    }

    double GetSize() const {
        return m_max - m_min;
    }

    static const Interval empty;
    static const Interval universe;

private:
    double m_min{};
    double m_max{};
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);