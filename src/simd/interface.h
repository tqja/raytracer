#pragma once

// Arithmetic
SIMD_REGISTER(void, Add, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, Add, const float* in1, const float* in2, float* out, size_t count)
SIMD_REGISTER(void, Add, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, Add, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, Sub, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, Sub, const float* in1, const float* in2, float* out, size_t count)
SIMD_REGISTER(void, Sub, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, Sub, const float* in, float scalar, float* out, size_t count)
SIMD_REGISTER(void, Sub, float scalar, float* inout, size_t count)
SIMD_REGISTER(void, Sub, float scalar, const float* in, float* out, size_t count)

SIMD_REGISTER(void, Mul, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, Mul, const float* in1, const float* in2, float* out, size_t count)
SIMD_REGISTER(void, Mul, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, Mul, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, Div, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, Div, const float* in1, const float* in2, float* out, size_t count)
SIMD_REGISTER(void, Div, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, Div, const float* in, float scalar, float* out, size_t count)
SIMD_REGISTER(void, Div, float scalar, float* inout, size_t count)
SIMD_REGISTER(void, Div, float scalar, const float* in, float* out, size_t count)

// Fused
SIMD_REGISTER(void, MulAdd, float* a, const float* b, const float* c, size_t count)
SIMD_REGISTER(void, MulAdd, const float* a, const float* b, const float* c, float* out, size_t count)
SIMD_REGISTER(void, MulAdd, float* a, float b, const float* c, size_t count)
SIMD_REGISTER(void, MulAdd, const float* a, float b, const float* c, float* out, size_t count)
SIMD_REGISTER(void, MulAdd, float* a, const float* b, float c, size_t count)
SIMD_REGISTER(void, MulAdd, const float* a, const float* b, float c, float* out, size_t count)
SIMD_REGISTER(void, MulAdd, float* a, float b, float c, size_t count)
SIMD_REGISTER(void, MulAdd, const float* a, float b, float c, float* out, size_t count)

SIMD_REGISTER(void, MulSub, float* a, const float* b, const float* c, size_t count)
SIMD_REGISTER(void, MulSub, const float* a, const float* b, const float* c, float* out, size_t count)
SIMD_REGISTER(void, MulSub, float* a, float b, const float* c, size_t count)
SIMD_REGISTER(void, MulSub, const float* a, float b, const float* c, float* out, size_t count)
SIMD_REGISTER(void, MulSub, float* a, const float* b, float c, size_t count)
SIMD_REGISTER(void, MulSub, const float* a, const float* b, float c, float* out, size_t count)
SIMD_REGISTER(void, MulSub, float* a, float b, float c, size_t count)
SIMD_REGISTER(void, MulSub, const float* a, float b, float c, float* out, size_t count)

// Unary
SIMD_REGISTER(void, Abs, float* inout, size_t count)
SIMD_REGISTER(void, Abs, const float* in, float* out, size_t count)
SIMD_REGISTER(void, Neg, float* inout, size_t count)
SIMD_REGISTER(void, Neg, const float* in, float* out, size_t count)
SIMD_REGISTER(void, Sqrt, float* inout, size_t count)
SIMD_REGISTER(void, Sqrt, const float* in, float* out, size_t count)
SIMD_REGISTER(void, Squared, float* inout, size_t count)
SIMD_REGISTER(void, Squared, const float* in, float* out, size_t count)

// Boolean logical
SIMD_REGISTER(void, And, float* inout, const float* mask_in, size_t count)
SIMD_REGISTER(void, And, const float* mask1, const float* mask2, float* out, size_t count)
SIMD_REGISTER(void, Or, float* inout, const float* mask_in, size_t count)
SIMD_REGISTER(void, Or, const float* mask1, const float* mask2, float* out, size_t count)
SIMD_REGISTER(void, Not, float* inout, size_t count)
SIMD_REGISTER(void, Not, const float* in, float* out, size_t count)

// Comparison
SIMD_REGISTER(void, LessThan, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, LessThan, const float* in, const float* vec, float* out, size_t count)
SIMD_REGISTER(void, LessThan, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, LessThan, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, LessEqual, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, LessEqual, const float* in, const float* vec, float* out, size_t count)
SIMD_REGISTER(void, LessEqual, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, LessEqual, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, GreaterThan, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, GreaterThan, const float* in, const float* vec, float* out, size_t count)
SIMD_REGISTER(void, GreaterThan, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, GreaterThan, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, GreaterEqual, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, GreaterEqual, const float* in, const float* vec, float* out, size_t count)
SIMD_REGISTER(void, GreaterEqual, float* inout, float scalar, size_t count)
SIMD_REGISTER(void, GreaterEqual, const float* in, float scalar, float* out, size_t count)

SIMD_REGISTER(void, Min, float* inout, const float* vec, const size_t count)
SIMD_REGISTER(void, Min, const float* in1, const float* in2, float* out, const size_t count)

SIMD_REGISTER(void, Min, float* inout, float scalar, const size_t count)
SIMD_REGISTER(void, Min, const float* in, float scalar, float* out, const size_t count)

SIMD_REGISTER(void, Max, float* inout, const float* vec, const size_t count)
SIMD_REGISTER(void, Max, const float* in1, const float* in2, float* out, const size_t count)

SIMD_REGISTER(void, Max, float* inout, float scalar, const size_t count)
SIMD_REGISTER(void, Max, const float* in, float scalar, float* out, const size_t count)

// Blend
SIMD_REGISTER(void, BlendScalars, float* mask_inout, float in1, float in2, size_t count)
SIMD_REGISTER(void, BlendScalars, const float* mask, float in1, float in2, float* out, size_t count)
SIMD_REGISTER(void, BlendVectors, const float* mask, float* inout, const float* vec, size_t count)
SIMD_REGISTER(void, BlendVectors, const float* mask, const float* in1, const float* in2, float* out, size_t count)
SIMD_REGISTER(void, BlendVectorScalar, const float* mask, float* inout, const float scalar, size_t count)
SIMD_REGISTER(void, BlendVectorScalar, const float* mask, const float* vec, const float scalar, float* out,
              size_t count)

// Misc
SIMD_REGISTER(float, ReduceSum, float* in, size_t count)
// SIMD_REGISTER(bool, AllBitsSet, const float* in, size_t count);
SIMD_REGISTER(bool, NoBitsSet, const float* in, size_t count);