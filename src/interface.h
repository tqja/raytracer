#pragma once

SIMD_REGISTER(void, Payload, )
SIMD_REGISTER(void, Add, const float* vec1, const float* vec2, float* out, size_t num)
SIMD_REGISTER(void, Add, const float* in, const float scalar, float* out, size_t num)
SIMD_REGISTER(void, Sub, const std::span<const float> vec1, const std::span<const float> vec2, 
    std::span<float> out, const size_t num)
SIMD_REGISTER(void, MulAdd, const std::span<const float> a, const std::span<const float> b,
    const std::span<const float> c, std::span<float> out, const size_t num)
