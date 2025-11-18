#include <cassert>
#include <span>
#include "simd.h"
#include "vec3.h"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "simd.cpp"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>


namespace simd {
namespace HWY_NAMESPACE {
namespace hn = hwy::HWY_NAMESPACE;

struct Dispatch : public simd::DispatchBase {
    #define SIMD_REGISTER(RET, NAME, ...) \
        virtual RET NAME(__VA_ARGS__) const = 0;
    #include "interface.h"
    #undef SIMD_REGISTER

    void Payload() const override {
        std::cout << "payload: target = "
            << hwy::TargetName(HWY_TARGET)
            << '\n';
    }

    void Add(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Add(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Add(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Add(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    void Sub(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Sub(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Sub(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Sub(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    void Mul(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Mul(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Mul(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Mul(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    template<typename Operation>
    void BinaryOp(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2, float* out, const size_t total_lanes, Operation op) const {
        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto v1{ hn::LoadU(d, in1 + i) };
            const auto v2{ hn::LoadU(d, in2 + i) };
            hn::StoreU(op(v1, v2), d, out + i);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto v1 = hn::MaskedLoad(mask, d, in1 + i);
            const auto v2 = hn::MaskedLoad(mask, d, in2 + i);
            hn::BlendedStore(op(v1, v2), mask, d, out + i);
        }
    }

    template<typename Operation>
    void BinaryOp(const float* HWY_RESTRICT in, const float scalar, float* out, const size_t total_lanes, Operation op) const {
        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        const auto v2{ hn::Set(d, scalar) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto v1{ hn::LoadU(d, in + i) };
            hn::StoreU(op(v1, v2), d, out + i);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto v1 = hn::MaskedLoad(mask, d, in + i);
            hn::BlendedStore(op(v1, v2), mask, d, out + i);
        }

    }

    /**
    * @brief Performs fused multiply-add for float arrays
    *
    * Computes: a[i] * b[i] + c[i] = out[i]
    *
    * @param[in] a First array to multiply
    * @param[in] b Second array to multiply
    * @param[in] c Array to add
    * @param[out] out Output array
    * @param[in] num Number of elements
    */
    void MulAdd(const std::span<const float> a, const std::span<const float> b, const std::span<const float> c,
        std::span<float> out, const size_t total_lanes) const {
        assert((a.size() >= 1) && "Input 'a' span <= 0 not allowed");
        assert((b.size() >= 1) && "Input 'b' span <= 0 not allowed");
        assert((c.size() >= 1) && "Input 'c' span <= 0 not allowed");

        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        const bool a_is_scalar{ a.size() == 1 };
        const bool b_is_scalar{ b.size() == 1 };
        const bool c_is_scalar{ c.size() == 1 };

        auto av{ a_is_scalar ? hn::Set(d, a[0]) : hn::Undefined(d)};
        auto bv{ b_is_scalar ? hn::Set(d, b[0]) : hn::Undefined(d)};
        auto cv{ c_is_scalar ? hn::Set(d, c[0]) : hn::Undefined(d)};

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            // if vector wasn't set with scalar, then load the values from array
            if (!a_is_scalar) { av = hn::LoadU(d, &a[i]); }
            if (!b_is_scalar) { bv = hn::LoadU(d, &b[i]); }
            if (!c_is_scalar) { cv = hn::LoadU(d, &c[i]); }
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            if (!a_is_scalar) { av = hn::MaskedLoad(mask, d, &a[i]); }
            if (!b_is_scalar) { bv = hn::MaskedLoad(mask, d, &b[i]); }
            if (!c_is_scalar) { cv = hn::MaskedLoad(mask, d, &c[i]); }
            hn::BlendedStore(hn::MulAdd(av, bv, cv), mask, d, &out[i]);
        }
    }

};

simd::DispatchBase* _GetDispatch()
{
    static Dispatch d;
    return &d;
}

}}

#if HWY_ONCE

namespace simd
{
    HWY_EXPORT(_GetDispatch);

    simd::DispatchBase* GetDispatch()
    {
        return HWY_DYNAMIC_DISPATCH(_GetDispatch)();
    }
}

#endif