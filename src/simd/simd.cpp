#include "simd.h"

// clang-format off
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "simd.cpp"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>
#include <hwy/contrib/algo/transform-inl.h>
// clang-format on

namespace simd {
namespace HWY_NAMESPACE {
namespace hn = hwy::HWY_NAMESPACE;

class Dispatch : public simd::DispatchBase {
public:
    HWY_ATTR void Add(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Add(lhs, rhs); });
    }

    HWY_ATTR void Add(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in1, count, in2, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Add(lhs, rhs); });
    }

    HWY_ATTR void Add(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Add(lhs, rhs); });
    }

    HWY_ATTR void Add(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Add(lhs, rhs); });
    }

    HWY_ATTR void Sub(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Sub(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in1, count, in2, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Sub(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Sub(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Sub(const float scalar, float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> lhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [lhs](auto /*d*/, auto rhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Sub(const float scalar, const float* HWY_RESTRICT in, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> lhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [lhs](auto /*d*/, auto rhs) { return hn::Sub(lhs, rhs); });
    }

    HWY_ATTR void Mul(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Mul(lhs, rhs); });
    }

    HWY_ATTR void Mul(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in1, count, in2, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Mul(lhs, rhs); });
    }

    HWY_ATTR void Mul(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Mul(lhs, rhs); });
    }

    HWY_ATTR void Mul(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Mul(lhs, rhs); });
    }

    HWY_ATTR void Div(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void Div(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in1, count, in2, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void Div(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void Div(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void Div(const float scalar, float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> lhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [lhs](auto /*d*/, auto rhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void Div(const float scalar, const float* HWY_RESTRICT in, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> lhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [lhs](auto /*d*/, auto rhs) { return hn::Div(lhs, rhs); });
    }

    HWY_ATTR void MulAdd(float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float* HWY_RESTRICT c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform2(d, a, count, b, c, [](auto /*d*/, auto va, auto vb, auto vc) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float* HWY_RESTRICT c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut2(d, a, count, b, c, out,
                      [](auto /*d*/, auto va, auto vb, auto vc) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(float* HWY_RESTRICT a, const float b, const float* HWY_RESTRICT c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        hn::Transform1(d, a, count, c, [vb](auto /*d*/, auto va, auto vc) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(const float* HWY_RESTRICT a, const float b, const float* HWY_RESTRICT c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        TransformOut1(d, a, count, c, out, [vb](auto /*d*/, auto va, auto vc) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        hn::Transform1(d, a, count, b, [vc](auto /*d*/, auto va, auto vb) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        TransformOut1(d, a, count, b, out, [vc](auto /*d*/, auto va, auto vb) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(float* HWY_RESTRICT a, const float b, const float c, const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        hn::Transform(d, a, count, [vb, vc](auto /*d*/, auto va) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulAdd(const float* HWY_RESTRICT a, const float b, const float c, float* HWY_RESTRICT out,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        TransformOut(d, a, count, out, [vb, vc](auto /*d*/, auto va) { return hn::MulAdd(va, vb, vc); });
    }

    HWY_ATTR void MulSub(float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float* HWY_RESTRICT c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform2(d, a, count, b, c, [](auto /*d*/, auto va, auto vb, auto vc) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float* HWY_RESTRICT c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut2(d, a, count, b, c, out,
                      [](auto /*d*/, auto va, auto vb, auto vc) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(float* HWY_RESTRICT a, const float b, const float* HWY_RESTRICT c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        hn::Transform1(d, a, count, c, [vb](auto /*d*/, auto va, auto vc) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(const float* HWY_RESTRICT a, const float b, const float* HWY_RESTRICT c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        TransformOut1(d, a, count, c, out, [vb](auto /*d*/, auto va, auto vc) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float c,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        hn::Transform1(d, a, count, b, [vc](auto /*d*/, auto va, auto vb) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float c,
                         float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        TransformOut1(d, a, count, b, out, [vc](auto /*d*/, auto va, auto vb) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(float* HWY_RESTRICT a, const float b, const float c, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        hn::Transform(d, a, count, [vb, vc](auto /*d*/, auto va) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void MulSub(const float* HWY_RESTRICT a, const float b, const float c, float* HWY_RESTRICT out,
                         const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const hn::Vec<decltype(d)> vb{ hn::Set(d, b) };
        const hn::Vec<decltype(d)> vc{ hn::Set(d, c) };
        TransformOut(d, a, count, out, [vb, vc](auto /*d*/, auto va) { return hn::MulSub(va, vb, vc); });
    }

    HWY_ATTR void Abs(float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform(d, inout, count, [](auto /*d*/, auto vec) { return hn::Abs(vec); });
    }

    HWY_ATTR void Abs(const float* HWY_RESTRICT in, float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut(d, in, count, out, [](auto /*d*/, auto vec) { return hn::Abs(vec); });
    }

    HWY_ATTR void Neg(float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform(d, inout, count, [](auto /*d*/, auto vec) { return hn::Neg(vec); });
    }

    HWY_ATTR void Neg(const float* HWY_RESTRICT in, float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut(d, in, count, out, [](auto /*d*/, auto vec) { return hn::Neg(vec); });
    }

    HWY_ATTR void Sqrt(float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform(d, inout, count, [](auto /*d*/, auto vec) { return hn::Sqrt(vec); });
    }

    HWY_ATTR void Sqrt(const float* HWY_RESTRICT in, float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut(d, in, count, out, [](auto /*d*/, auto vec) { return hn::Sqrt(vec); });
    }

    HWY_ATTR void Squared(float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform(d, inout, count, [](auto /*d*/, auto vec) { return hn::Mul(vec, vec); });
    }

    HWY_ATTR void Squared(const float* HWY_RESTRICT in, float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut(d, in, count, out, [](auto /*d*/, auto vec) { return hn::Mul(vec, vec); });
    }

    HWY_ATTR void And(float* HWY_RESTRICT inout, const float* HWY_RESTRICT mask, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, mask, [](auto /*d*/, auto lhs, auto rhs) { return hn::And(lhs, rhs); });
    }

    HWY_ATTR void And(const float* HWY_RESTRICT in, const float* HWY_RESTRICT mask, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in, count, mask, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::And(lhs, rhs); });
    }

    HWY_ATTR void Or(float* HWY_RESTRICT inout, const float* HWY_RESTRICT mask, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, mask, [](auto /*d*/, auto lhs, auto rhs) { return hn::Or(lhs, rhs); });
    }

    HWY_ATTR void Or(const float* HWY_RESTRICT in, const float* HWY_RESTRICT mask, float* HWY_RESTRICT out,
                     const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in, count, mask, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Or(lhs, rhs); });
    }

    HWY_ATTR void Not(float* HWY_RESTRICT inout, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform(d, inout, count, [](auto /*d*/, auto mask) { return hn::Not(mask); });
    }

    HWY_ATTR void Not(const float* HWY_RESTRICT in, float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut(d, in, count, out, [](auto /*d*/, auto mask) { return hn::Not(mask); });
    }

    HWY_ATTR void LessThan(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec,
                           const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare1(d, inout, count, vec, [](auto lhs, auto rhs) { return hn::Lt(lhs, rhs); });
    }

    HWY_ATTR void LessThan(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                           const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut1(d, in1, count, in2, out, [](auto lhs, auto rhs) { return hn::Lt(lhs, rhs); });
    }

    HWY_ATTR void LessThan(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare(d, inout, count, scalar, [](auto lhs, auto rhs) { return hn::Lt(lhs, rhs); });
    }

    HWY_ATTR void LessThan(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                           const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut(d, in, count, scalar, out, [](auto lhs, auto rhs) { return hn::Lt(lhs, rhs); });
    }

    HWY_ATTR void LessEqual(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec,
                            const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare1(d, inout, count, vec, [](auto lhs, auto rhs) { return hn::Le(lhs, rhs); });
    }

    HWY_ATTR void LessEqual(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                            const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut1(d, in1, count, in2, out, [](auto lhs, auto rhs) { return hn::Le(lhs, rhs); });
    }

    HWY_ATTR void LessEqual(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare(d, inout, count, scalar, [](auto lhs, auto rhs) { return hn::Le(lhs, rhs); });
    }

    HWY_ATTR void LessEqual(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                            const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut(d, in, count, scalar, out, [](auto lhs, auto rhs) { return hn::Le(lhs, rhs); });
    }

    HWY_ATTR void GreaterThan(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec,
                              const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare1(d, inout, count, vec, [](auto lhs, auto rhs) { return hn::Gt(lhs, rhs); });
    }

    HWY_ATTR void GreaterThan(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                              const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut1(d, in1, count, in2, out, [](auto lhs, auto rhs) { return hn::Gt(lhs, rhs); });
    }

    HWY_ATTR void GreaterThan(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare(d, inout, count, scalar, [](auto lhs, auto rhs) { return hn::Gt(lhs, rhs); });
    }

    HWY_ATTR void GreaterThan(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                              const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut(d, in, count, scalar, out, [](auto lhs, auto rhs) { return hn::Gt(lhs, rhs); });
    }

    HWY_ATTR void GreaterEqual(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec,
                               const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare1(d, inout, count, vec, [](auto lhs, auto rhs) { return hn::Ge(lhs, rhs); });
    }

    HWY_ATTR void GreaterEqual(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                               const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut1(d, in1, count, in2, out, [](auto lhs, auto rhs) { return hn::Ge(lhs, rhs); });
    }

    HWY_ATTR void GreaterEqual(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        Compare(d, inout, count, scalar, [](auto lhs, auto rhs) { return hn::Ge(lhs, rhs); });
    }

    HWY_ATTR void GreaterEqual(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                               const size_t count) const override {
        const hn::ScalableTag<float> d{};
        CompareOut(d, in, count, scalar, out, [](auto lhs, auto rhs) { return hn::Ge(lhs, rhs); });
    }

    HWY_ATTR void Min(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Min(lhs, rhs); });
    }

    HWY_ATTR void Min(const float* HWY_RESTRICT in, const float* HWY_RESTRICT vec, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in, count, vec, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Min(lhs, rhs); });
    }

    HWY_ATTR void Min(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Min(lhs, rhs); });
    }

    HWY_ATTR void Min(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Min(lhs, rhs); });
    }

    HWY_ATTR void Max(float* HWY_RESTRICT inout, const float* HWY_RESTRICT vec, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        hn::Transform1(d, inout, count, vec, [](auto /*d*/, auto lhs, auto rhs) { return hn::Max(lhs, rhs); });
    }

    HWY_ATTR void Max(const float* HWY_RESTRICT in, const float* HWY_RESTRICT vec, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        TransformOut1(d, in, count, vec, out, [](auto /*d*/, auto lhs, auto rhs) { return hn::Max(lhs, rhs); });
    }

    HWY_ATTR void Max(float* HWY_RESTRICT inout, const float scalar, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [rhs](auto /*d*/, auto lhs) { return hn::Max(lhs, rhs); });
    }

    HWY_ATTR void Max(const float* HWY_RESTRICT in, const float scalar, float* HWY_RESTRICT out,
                      const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> rhs{ hn::Set(d, scalar) };
        TransformOut(d, in, count, out, [rhs](auto /*d*/, auto lhs) { return hn::Max(lhs, rhs); });
    }

    HWY_ATTR float ReduceSum(float* HWY_RESTRICT in, const size_t count) const override {
        hn::ScalableTag<float> d{};
        const size_t N{ hn::Lanes(d) };

        auto sum{ hn::Zero(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                hn::Vec<decltype(d)> vec{ hn::LoadU(d, in + i) };
                sum = hn::Add(sum, vec);
            }
        }

        if (HWY_UNLIKELY(i == count)) return hn::ReduceSum(d, sum);

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<decltype(d)> vec = hn::LoadN(d, in + i, remaining);
        sum = hn::Add(sum, vec);

        return hn::ReduceSum(d, sum);
    }

    HWY_ATTR void BlendScalars(float* HWY_RESTRICT mask_inout, const float in1, const float in2,
                               const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const hn::Vec<decltype(d)> v1{ hn::Set(d, in1) };
        const hn::Vec<decltype(d)> v2{ hn::Set(d, in2) };

        hn::Transform(d, mask_inout, count, [v1, v2](auto d, auto mask) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(mask) };
            return hn::IfThenElse(m, v1, v2);
        });
    }

    HWY_ATTR void BlendScalars(const float* HWY_RESTRICT mask, const float in1, const float in2,
                               float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const hn::Vec<decltype(d)> v1{ hn::Set(d, in1) };
        const hn::Vec<decltype(d)> v2{ hn::Set(d, in2) };

        TransformOut(d, mask, count, out, [v1, v2](auto d, auto mask) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(mask) };
            return hn::IfThenElse(m, v1, v2);
        });
    }

    HWY_ATTR void BlendVectors(const float* mask, float* inout, const float* vec, size_t count) const override {
        const hn::ScalableTag<float> d{};

        hn::Transform2(d, inout, count, vec, mask, [](auto d, auto v1, auto v2, auto m1) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(m1) };
            return hn::IfThenElse(m, v1, v2);
        });
    }

    HWY_ATTR void BlendVectors(const float* HWY_RESTRICT mask, const float* HWY_RESTRICT in1,
                               const float* HWY_RESTRICT in2, float* HWY_RESTRICT out,
                               const size_t count) const override {
        const hn::ScalableTag<float> d{};

        TransformOut2(d, mask, count, in1, in2, out, [](auto d, auto mask, auto v1, auto v2) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(mask) };
            return hn::IfThenElse(m, v1, v2);
        });
    }

    HWY_ATTR void BlendVectorScalar(const float* mask, float* inout, const float scalar, size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> v_scalar{ hn::Set(d, scalar) };

        hn::Transform1(d, inout, count, mask, [v_scalar](auto d, auto v_vec, auto m1) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(m1) };
            return hn::IfThenElse(m, v_scalar, v_vec);
        });
    }

    HWY_ATTR void BlendVectorScalar(const float* HWY_RESTRICT mask, const float* HWY_RESTRICT vec, const float scalar,
                                    float* HWY_RESTRICT out, const size_t count) const override {
        const hn::ScalableTag<float> d{};
        const hn::Vec<decltype(d)> v_scalar{ hn::Set(d, scalar) };

        TransformOut1(d, mask, count, vec, out, [v_scalar](auto d, auto mask, auto v_vec) {
            const hn::Mask<decltype(d)> m{ hn::MaskFromVec(mask) };
            return hn::IfThenElse(m, v_scalar, v_vec);
        });
    }

    /*HWY_ATTR bool AllBitsSet(const float* HWY_RESTRICT in, const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const size_t N{ hn::Lanes(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                const hn::Vec<float> v{ hn::LoadU(d, in + i) };
                if (!hn::AllBits1(d, v)) return false;
            }
        }

        if (HWY_UNLIKELY(i == count)) return true;

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<float> v{ hn::LoadN(d, in + i, remaining) };
        return hn::AllBits1(d, v);
    }*/

    HWY_ATTR bool NoBitsSet(const float* HWY_RESTRICT in, const size_t count) const override {
        const hn::ScalableTag<float> d{};

        const size_t N{ hn::Lanes(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                const hn::Vec<decltype(d)> v{ hn::LoadU(d, in + i) };
                if (!hn::AllBits0(d, v)) return false;
            }
        }

        if (HWY_UNLIKELY(i == count)) return true;

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<decltype(d)> v{ hn::LoadN(d, in + i, remaining) };
        return hn::AllBits0(d, v);
    }

private:
    template <class D, typename T = hn::TFromD<D>, class Func>
    HWY_ATTR void TransformOut(D d, const T* HWY_RESTRICT in, const size_t count, T* HWY_RESTRICT out,
                               const Func& func) const {
        HWY_DASSERT(in != out && "Input pointer cannot alias out pointer");

        const size_t N{ hn::Lanes(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                const hn::Vec<D> v{ hn::LoadU(d, in + i) };
                hn::StoreU(func(d, v), d, out + i);
            }
        }

        if (HWY_UNLIKELY(i == count)) return;

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<D> v{ hn::LoadN(d, in + i, remaining) };
        hn::StoreN(func(d, v), d, out + i, remaining);
    }

    template <class D, typename T = hn::TFromD<D>, class Func>
    HWY_ATTR void TransformOut1(D d, const T* HWY_RESTRICT in1, const size_t count, const T* HWY_RESTRICT in2,
                                T* HWY_RESTRICT out, const Func& func) const {
        HWY_DASSERT(in1 != out && in2 != out && "Input pointers cannot alias out pointer");

        const size_t N{ hn::Lanes(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                const hn::Vec<D> v1{ hn::LoadU(d, in1 + i) };
                const hn::Vec<D> v2{ hn::LoadU(d, in2 + i) };
                hn::StoreU(func(d, v1, v2), d, out + i);
            }
        }

        if (HWY_UNLIKELY(i == count)) return;

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<D> v1{ hn::LoadN(d, in1 + i, remaining) };
        const hn::Vec<D> v2{ hn::LoadN(d, in2 + i, remaining) };
        hn::StoreN(func(d, v1, v2), d, out + i, remaining);
    }

    template <class D, typename T = hn::TFromD<D>, class Func>
    HWY_ATTR void TransformOut2(D d, const T* HWY_RESTRICT in1, const size_t count, const T* HWY_RESTRICT in2,
                                const T* HWY_RESTRICT in3, T* HWY_RESTRICT out, const Func& func) const {
        HWY_DASSERT(in1 != out && in2 != out && in3 != out && "Input pointers cannot alias out pointer");

        const size_t N{ hn::Lanes(d) };

        size_t i{ 0 };
        if (count >= N) {
            for (; i <= count - N; i += N) {
                const hn::Vec<D> v1{ hn::LoadU(d, in1 + i) };
                const hn::Vec<D> v2{ hn::LoadU(d, in2 + i) };
                const hn::Vec<D> v3{ hn::LoadU(d, in3 + i) };
                hn::StoreU(func(d, v1, v2, v3), d, out + i);
            }
        }

        if (HWY_UNLIKELY(i == count)) return;

        const size_t remaining{ count - i };
        HWY_DASSERT(0 != remaining && remaining < N);
        const hn::Vec<D> v1{ hn::LoadN(d, in1 + i, remaining) };
        const hn::Vec<D> v2{ hn::LoadN(d, in2 + i, remaining) };
        const hn::Vec<D> v3{ hn::LoadN(d, in3 + i, remaining) };
        hn::StoreN(func(d, v1, v2, v3), d, out + i, remaining);
    }

    template <typename D, typename T = hn::TFromD<D>, typename Func>
    HWY_ATTR void Compare(D d, T* HWY_RESTRICT inout, const size_t count, const T scalar, const Func& func) const {
        hn::Vec<D> rhs{ hn::Set(d, scalar) };
        hn::Transform(d, inout, count, [func, rhs](auto d, auto lhs) {
            const auto mask{ func(lhs, rhs) };
            return hn::VecFromMask(d, mask);
        });
    }

    template <typename D, typename T = hn::TFromD<D>, typename Func>
    HWY_ATTR void Compare1(D d, T* HWY_RESTRICT inout, const size_t count, const T* HWY_RESTRICT vec,
                           const Func& func) const {
        hn::Transform1(d, inout, count, vec, [func](auto d, auto lhs, auto rhs) {
            const auto mask{ func(lhs, rhs) };
            return hn::VecFromMask(d, mask);
        });
    }

    template <typename D, typename T = hn::TFromD<D>, typename Func>
    HWY_ATTR void CompareOut(D d, const T* HWY_RESTRICT in, const size_t count, const T scalar, T* HWY_RESTRICT out,
                             const Func& func) const {
        hn::Vec<D> rhs{ hn::Set(d, scalar) };
        this->TransformOut(d, in, count, out, [func, rhs](auto d, auto lhs) {
            const auto mask{ func(lhs, rhs) };
            return hn::VecFromMask(d, mask);
        });
    }

    template <typename D, typename T = hn::TFromD<D>, typename Func>
    HWY_ATTR void CompareOut1(D d, const T* HWY_RESTRICT in1, const size_t count, const T* HWY_RESTRICT in2,
                              T* HWY_RESTRICT out, const Func& func) const {
        this->TransformOut1(d, in1, count, in2, out, [func](auto d, auto lhs, auto rhs) {
            const auto mask{ func(lhs, rhs) };
            return hn::VecFromMask(d, mask);
        });
    }
};

static simd::DispatchBase* _GetDispatch() {
    static Dispatch dispatch;
    return &dispatch;
}

}  // namespace HWY_NAMESPACE
}  // namespace simd

#if HWY_ONCE

namespace simd {

HWY_EXPORT(_GetDispatch);

simd::DispatchBase* GetDispatch() { return HWY_DYNAMIC_DISPATCH(_GetDispatch)(); }

}  // namespace simd

#endif