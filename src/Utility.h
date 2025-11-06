#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

using std::make_shared;
using std::shared_ptr;

constexpr double infinity{ std::numeric_limits<double>::infinity() };
constexpr double pi{ 3.1415926535897932385 };

inline double DegToRads(double degrees) {
    return degrees * pi / 180.0;
}

inline double RandomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

class Timer {
public:
    void reset() {
        m_start = Clock::now();
    }

    double elapsed() const {
        return std::chrono::duration_cast<Second>(Clock::now() - m_start).count();
    }

private:
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<Clock> m_start{ Clock::now() };
};
