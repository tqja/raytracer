#pragma once

SIMD_REGISTER(void, Payload, )
SIMD_REGISTER(void, Add, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Add, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, Sub, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Sub, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, Sub, const float scalar, const float* vec, float* out, const size_t total_lanes)
SIMD_REGISTER(void, Mul, const float* in1, const float* in2, float* out, size_t total_lanes)
SIMD_REGISTER(void, Mul, const float* vec, const float scalar, float* out, const size_t total_lanes)
SIMD_REGISTER(void, MulAdd, const float* a, const float* b, const float* c, float* out, size_t total_lanes)
SIMD_REGISTER(void, MulAdd, const float* a, const float b, const float* c, float* out, size_t total_lanes)
SIMD_REGISTER(void, MulAdd, const float* a, const float* b, const float c, float* out, size_t total_lanes)
SIMD_REGISTER(void, MulAdd, const float* a, const float b, const float c, float* out, size_t total_lanes)
SIMD_REGISTER(float, ReduceSum, const float* in, int total_lanes);
SIMD_REGISTER(void, Squared, const float* in, float* out, size_t total_lanes)
SIMD_REGISTER(void, Sqrt, const float* in, float* out, size_t total_lanes)
SIMD_REGISTER(void, Reciprocal, const float* in, float* out, size_t total_lanes)