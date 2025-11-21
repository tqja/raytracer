#include <cassert>
#include <span>
#include <iostream>

#include "simd.h"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "simd.cpp"

#pragma warning(push)
#pragma warning(disable: 4365)

#include <hwy/foreach_target.h>
#include <hwy/highway.h>

#pragma warning(pop)


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

    void Add(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, float* out, const size_t total_lanes
    ) const override {
        const auto op = [](auto in1, auto in2) { return hn::Add(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Add(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Add(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    void Sub(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2,
        float* out, const size_t total_lanes
    ) const override {
        const auto op = [](auto in1, auto in2) { return hn::Sub(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Sub(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Sub(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    void Sub(const float scalar, const float* HWY_RESTRICT vec,
        float* HWY_RESTRICT out, const size_t total_lanes
    ) const override {
        const auto op = [](auto in1, auto in2) { return hn::Sub(in1, in2); };
        BinaryOp(scalar, vec, out, total_lanes, op);
    }

    void Mul(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, 
        float* out, const size_t total_lanes
    ) const override {
        const auto op = [](auto in1, auto in2) { return hn::Mul(in1, in2); };
        BinaryOp(vec1, vec2, out, total_lanes, op);
    }

    void Mul(const float* HWY_RESTRICT vec, const float scalar, float* out, const size_t total_lanes) const override {
        const auto op = [](auto in1, auto in2) { return hn::Mul(in1, in2); };
        BinaryOp(vec, scalar, out, total_lanes, op);
    }

    template<typename Operation>
    void BinaryOp(const float* HWY_RESTRICT in1, const float* HWY_RESTRICT in2,
        float* out, const size_t total_lanes, Operation op
    ) const {
        const hn::ScalableTag<float> d{};
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
    void BinaryOp(const float* HWY_RESTRICT in, const float scalar,
        float* out, const size_t total_lanes, Operation op
    ) const {
        const hn::ScalableTag<float> d{};
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

    void MulAdd(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float* HWY_RESTRICT c,
        float* HWY_RESTRICT out, size_t total_lanes
    ) const override {
        const hn::ScalableTag<float> d{};
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto av{ hn::LoadU(d, &a[i]) };
            const auto bv{ hn::LoadU(d, &b[i]) };
            const auto cv{ hn::LoadU(d, &c[i]) };
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto av{ hn::MaskedLoad(mask, d, &a[i]) };
            const auto bv{ hn::MaskedLoad(mask, d, &b[i]) };
            const auto cv{ hn::MaskedLoad(mask, d, &c[i]) };
            hn::BlendedStore(hn::MulAdd(av, bv, cv), mask, d, &out[i]);
        }
    }

    void MulAdd(const float* HWY_RESTRICT a, const float b, const float* HWY_RESTRICT c,
        float* HWY_RESTRICT out, size_t total_lanes
    ) const override {
        const hn::ScalableTag<float> d{};
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        const auto bv{ hn::Set(d, b) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto av{ hn::LoadU(d, &a[i]) };
            const auto cv{ hn::LoadU(d, &c[i]) };
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto av{ hn::MaskedLoad(mask, d, &a[i]) };
            const auto cv{ hn::MaskedLoad(mask, d, &c[i]) };
            hn::BlendedStore(hn::MulAdd(av, bv, cv), mask, d, &out[i]);
        }
    }

    void MulAdd(const float* HWY_RESTRICT a, const float* HWY_RESTRICT b, const float c,
        float* HWY_RESTRICT out, size_t total_lanes
    ) const override {
        const hn::ScalableTag<float> d{};
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        const auto cv{ hn::Set(d, c) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto av{ hn::LoadU(d, &a[i]) };
            const auto bv{ hn::LoadU(d, &b[i]) };
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto av{ hn::MaskedLoad(mask, d, &a[i]) };
            const auto bv{ hn::MaskedLoad(mask, d, &b[i]) };
            hn::BlendedStore(hn::MulAdd(av, bv, cv), mask, d, &out[i]);
        }
    }

    void MulAdd(const float* HWY_RESTRICT a, const float b, const float c, float* HWY_RESTRICT out, size_t total_lanes
    ) const override { 
        const hn::ScalableTag<float> d{};
        const size_t N{ hn::Lanes(d) };
        const size_t aligned_lanes{ total_lanes & ~(N - 1) };

        const auto bv{ hn::Set(d, b) };
        const auto cv{ hn::Set(d, c) };

        size_t i = 0;
        for (; i < aligned_lanes; i += N) {
            const auto av{ hn::LoadU(d, &a[i]) };
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < total_lanes) {
            const size_t remaining_lanes{ total_lanes - i };
            auto mask{ hn::FirstN(d, remaining_lanes) };
            const auto av{ hn::MaskedLoad(mask, d, &a[i]) };
            hn::BlendedStore(hn::MulAdd(av, bv, cv), mask, d, &out[i]);
        }
    }

};

static simd::DispatchBase* _GetDispatch()
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