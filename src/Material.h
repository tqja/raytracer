#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "Colour.h"
#include "ColourConstants.h"

#include <functional>
#include <numeric>
#include <vector>

class Material {
public:
    virtual ~Material() = default;

    virtual bool Scatter(
        const Ray&, const HitRecord&, Colour&, Ray&
    ) const {
        return false;
    }

};


class Lambertian : public Material {
public:
    Lambertian(const Colour& albedo) : m_albedo(albedo) {};

    bool Scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    Colour m_albedo{};
};


class Metal : public Material {
public:
    Metal(const Colour& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    bool Scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    Colour m_albedo{};
    float m_fuzz{};
};


class Dielectric : public Material {
public:
    Dielectric(float refraction_index) : m_refraction_index(refraction_index) {}

    bool Scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    float m_refraction_index;

    static float Reflectance(float cosine, float refraction_index);
};


inline shared_ptr<Material> RandomMaterial() {
    struct MaterialChoice {
        float weight{};
        std::function<shared_ptr<Material>()> make;
    };

    static auto MakeDiffuse = []() {
        Colour albedo{ Colour::Random() * Colour::Random() };
        return make_shared<Lambertian>(albedo);
        };
    static auto MakeMetal = []() {
        const float colour_low{ 0.5f };
        const float colour_high{ 1.0f };
        const auto albedo{ Colour::Random(colour_low, colour_high) };

        const float fuzz_low{ 0.0f };
        const float fuzz_high{ 0.5f };
        const auto fuzz{ RandomFloat(fuzz_low, fuzz_high) };

        return make_shared<Metal>(albedo, fuzz);
        };
    static auto MakeGlass = []() {
        const float refraction_index{ 1.5f };
        return make_shared<Dielectric>(refraction_index);
        };

    static const std::vector<MaterialChoice> materials{
        {0.4f, MakeDiffuse},
        {0.4f, MakeMetal},
        {0.2f, MakeGlass},
    };

    static const float total_weight{
        std::accumulate(materials.begin(), materials.end(), 0.0f,
                [](float sum, const MaterialChoice& choice) { return sum + choice.weight; })
    };

    auto p_material{ RandomFloat() * total_weight };
    float p_cumulative{ 0.0f };
    for (const auto& choice : materials) {
        p_cumulative += choice.weight;
        if (p_material < p_cumulative) {
            return choice.make();
        }
    }

    // fallback - select final material in vector
    return materials.back().make();
}