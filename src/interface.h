#pragma once

SIMD_REGISTER(void, Payload, )
SIMD_REGISTER(void, Add, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Add, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, Sub, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Sub, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, Mul, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Mul, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, MulAdd, const std::span<const float> a, const std::span<const float> b,
    const std::span<const float> c, std::span<float> out, const size_t num)
