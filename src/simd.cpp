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

struct Dispatch : public simd::DispatchBase
{
    #define SIMD_REGISTER(RET, NAME, ...) \
        virtual RET NAME(__VA_ARGS__) const = 0;
    #include "interface.h"
    #undef SIMD_REGISTER

    void Payload() const override
    {
        std::cout << "payload: target = "
            << hwy::TargetName(HWY_TARGET)
            << '\n';
    }

    /**
    * @brief Adds two arrays of floats together
    * 
    * Computes: vec1[i] + vec2[i] = out[i]
    * 
    * @param[in] vec1 LHS operand for addition
    * @param[in] vec2 RHS operand for addition
    * @param[out] out Output vector (result)
    * @param[in] num Number of elements
    */
    void Add(const float* HWY_RESTRICT vec1, const float* HWY_RESTRICT vec2, float* out, const size_t num) const override
    {
        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t num_full_lanes = num - num % N;  // max iterations that evenly divide N

        size_t i = 0;
        for (; i < num_full_lanes; i += N)
        {
            const auto v1{ hn::LoadU(d, vec1 + i) };
            const auto v2{ hn::LoadU(d, vec2 + i) };
            hn::StoreU(hn::Add(v1, v2), d, out + i);
        }

        // process the remainder
        if (i < num) {
            auto mask{ hn::FirstN(d, num - i) };
            const auto v1 = hn::MaskedLoad(mask, d, vec1 + i);
            const auto v2 = hn::MaskedLoad(mask, d, vec2 + i);
            hn::BlendedStore(hn::Add(v1, v2), mask, d, out + i);
        }
    }

    /**
    * @brief Adds a scalar to a vector of floats
    *
    * Computes: in[i] + scalar = out[i]
    *
    * @param[in] in The vector of floats
    * @param[in] scalar The value to add to the vector
    * @param[out] out Output vector (result)
    * @param[in] num Number of elements
    */
    void Add(const float* HWY_RESTRICT in, const float scalar, float* out, const size_t num) const override
    {
        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t num_full_lanes = num - num % N;
        const auto v2{ hn::Set(d, scalar) };

        size_t i = 0;
        for (; i < num_full_lanes; i += N)
        {
            const auto v1{ hn::LoadU(d, in + i) };
            hn::StoreU(hn::Add(v1, v2), d, out + i);
        }

        // process the remainder
        if (i < num) {
            auto mask{ hn::FirstN(d, num - i) };
            const auto v1 = hn::MaskedLoad(mask, d, in + i);
            hn::BlendedStore(hn::Add(v1, v2), mask, d, out + i);
        }

    }

    void Sub(const std::span<const float> vec1, const std::span<const float> vec2, std::span<float> out,
        const size_t num) const
    {
        assert((vec1.size() >= 1) && "Input 'vec1' span <= 0 not allowed");
        assert((vec2.size() >= 1) && "Input 'vec2' span <= 0 not allowed");

        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t num_full_lanes = num - num % N;  // max iterations that evenly divide N

        const bool vec1_is_scalar{ vec1.size() == 1 };
        const bool vec2_is_scalar{ vec2.size() == 1 };

        auto v1{ vec1_is_scalar ? hn::Set(d, vec1[0]) : hn::Undefined(d) };
        auto v2{ vec2_is_scalar ? hn::Set(d, vec2[0]) : hn::Undefined(d) };

        size_t i = 0;
        for (; i < num_full_lanes; i += N)
        {
            if (!vec1_is_scalar) { v1 = hn::LoadU(d, &vec1[i]); }
            if (!vec2_is_scalar) { v2 = hn::LoadU(d, &vec2[i]); }
            hn::StoreU(hn::Sub(v1, v2), d, &out[i]);
        }

        // process the remainder
        if (i < num) {
            auto mask{ hn::FirstN(d, num - i) };
            if (!vec1_is_scalar) { v1 = hn::MaskedLoad(mask, d, &vec1[i]); }
            if (!vec2_is_scalar) { v2 = hn::MaskedLoad(mask, d, &vec2[i]); }
            hn::BlendedStore(hn::Sub(v1, v2), mask, d, &out[i]);
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
        std::span<float> out, const size_t num) const
    {
        assert((a.size() >= 1) && "Input 'a' span <= 0 not allowed");
        assert((b.size() >= 1) && "Input 'b' span <= 0 not allowed");
        assert((c.size() >= 1) && "Input 'c' span <= 0 not allowed");

        const hn::ScalableTag<float> d;
        const size_t N{ hn::Lanes(d) };
        const size_t num_full_lanes = num - num % N;

        const bool a_is_scalar{ a.size() == 1 };
        const bool b_is_scalar{ b.size() == 1 };
        const bool c_is_scalar{ c.size() == 1 };

        auto av{ a_is_scalar ? hn::Set(d, a[0]) : hn::Undefined(d)};
        auto bv{ b_is_scalar ? hn::Set(d, b[0]) : hn::Undefined(d)};
        auto cv{ c_is_scalar ? hn::Set(d, c[0]) : hn::Undefined(d)};

        size_t i = 0;
        for (; i < num_full_lanes; i += N) {
            // if vector wasn't set with scalar, then load the values from array
            if (!a_is_scalar) { av = hn::LoadU(d, &a[i]); }
            if (!b_is_scalar) { bv = hn::LoadU(d, &b[i]); }
            if (!c_is_scalar) { cv = hn::LoadU(d, &c[i]); }
            hn::StoreU(hn::MulAdd(av, bv, cv), d, &out[i]);
        }

        if (i < num) {
            auto mask{ hn::FirstN(d, num - i) };
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