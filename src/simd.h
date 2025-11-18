#pragma once
#include "vec3.h"
#include <span>

namespace simd
{
    struct DispatchBase
    {
        #define SIMD_REGISTER(RET,NAME,...) \
            virtual RET NAME(__VA_ARGS__) const = 0;
        #include "interface.h"
        #undef SIMD_REGISTER

        /**
        * @brief Performs fused multiply-add on Vec3Group/Vec3 components
        *
        * Computes a * b + c = out for each component (x, y, z) of Vec3Group.
        * Passes Vec3 components as a scalar instead, to be broadcast into a vector.
        *
        * @param[in] a First Vec to multiply
        * @param[in] b Second Vec to multiply
        * @param[in] c Vec to add
        * @param[out] out Output group
        * @param[in] num Number of elements
        */

        template <typename T>
        static std::span<float> GetComponentData(T& t, int axis) {
            using BaseT = std::decay_t<T>;

            if constexpr (std::is_same_v<BaseT, Vec3Group>) {
                switch (axis) {
                case 0: return std::span(t.x.data(), t.x.size());
                case 1: return std::span(t.y.data(), t.y.size());
                default: return std::span(t.z.data(), t.z.size());
                }
            }
            else if constexpr (std::is_same_v<BaseT, Vec3>) {
                switch (axis) {
                case 0: return std::span(&t.e[0], 1);
                case 1: return std::span(&t.e[1], 1);
                default: return std::span(&t.e[2], 1);
                }
            }
        }

        template <typename T>
        static std::span<const float> GetComponentData(const T& t, int axis) {
            using BaseT = std::decay_t<T>;

            if constexpr (std::is_same_v<BaseT, Vec3Group>) {
                switch (axis) {
                case 0: return std::span(t.x.data(), t.x.size());
                case 1: return std::span(t.y.data(), t.y.size());
                default: return std::span(t.z.data(), t.z.size());
                }
            }
            else if constexpr(std::is_same_v<BaseT, Vec3>) {
                switch (axis) {
                case 0: return std::span(&t.e[0], 1);
                case 1: return std::span(&t.e[1], 1);
                default: return std::span(&t.e[2], 1);
                }
            }
        }
    };

    DispatchBase* GetDispatch();
}
