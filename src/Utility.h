#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

using std::make_shared;
using std::shared_ptr;

constexpr float infinity{ std::numeric_limits<float>::infinity() };
constexpr float pi{ 3.1415927f };

inline float DegToRads(float degrees) {
    return degrees * pi / 180.0f;
}

inline float RandomFloat() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float RandomFloat(float min, float max) {
    return min + (max - min) * RandomFloat();
}

class Timer {
public:
    void reset() {
        m_start = Clock::now();
    }

    float elapsed() const {
        return std::chrono::duration_cast<Second>(Clock::now() - m_start).count();
    }

private:
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<float, std::ratio<1>>;

    std::chrono::time_point<Clock> m_start{ Clock::now() };
};


namespace globals {
    constexpr size_t lane_width{ 8 };
    constexpr int samples{ 500 };
}
