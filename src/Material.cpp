#include "Material.h"

#include <functional>
#include <memory>
#include <numeric>
#include <vector>

#include "ColourConstants.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"
#include "Vec3.h"
#include "colour.h"
#include "simd/simd.h"

void Lambertian::Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const {
    Vec3Group random_unit_vectors{};
    random_unit_vectors.MaskedGenerate(RandomUnitVector, records.GetHitMask());

    Vec3Group scatter_directions{};
    records.GetNormals().Add(random_unit_vectors, scatter_directions);

    rays.ReplaceOrigins(records.GetHitPoints(), records.GetHitMask());

    simd::DispatchBase* dp{ simd::GetDispatch() };
    std::vector<float> hit_near_zero_mask(rays.Size());
    dp->And(records.GetHitMask().data(), scatter_directions.NearZero().data(), hit_near_zero_mask.data(),
            globals::samples);

    rays.ReplaceDirections(records.GetNormals(), hit_near_zero_mask);
    rays.ReplaceDirections(scatter_directions, records.GetHitMask());

    attenuation = m_albedo;
}

void Metal::Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const {
    Vec3Group random_unit_vectors{};
    random_unit_vectors.MaskedGenerate(RandomUnitVector, records.GetHitMask());

    Vec3Group reflected{};
    rays.GetDirections().Reflect(records.GetNormals(), reflected);

    reflected.UnitVectors();
    random_unit_vectors.MulScalar(m_fuzz);
    reflected.Add(random_unit_vectors);

    rays.ReplaceOrigins(records.GetHitPoints(), records.GetHitMask());
    rays.ReplaceDirections(reflected, records.GetHitMask());
    attenuation = m_albedo;

    std::vector<float> dot_products(rays.Size());
    rays.GetDirections().Dot(records.GetNormals(), dot_products);
}

void Dielectric::Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const {
    attenuation = Colours::white;

    simd::DispatchBase* dp{ simd::GetDispatch() };
    std::vector<float> refractive_indices(rays.Size());
    dp->BlendScalars(records.GetFrontFace().data(), 1.0f / m_refraction_index, m_refraction_index,
                     refractive_indices.data(), globals::samples);

    Vec3Group unit_directions{ rays.GetDirections() };
    unit_directions.UnitVectors(unit_directions);

    Vec3Group neg_unit_directions{};
    unit_directions.Neg(neg_unit_directions);

    std::vector<float> dot_products(rays.Size());
    neg_unit_directions.Dot(records.GetNormals(), dot_products);

    std::vector<float> cos_theta(rays.Size());
    dp->Min(dot_products.data(), 1.0f, cos_theta.data(), globals::samples);

    std::vector<float> sin_theta(rays.Size());
    dp->Mul(cos_theta.data(), cos_theta.data(), sin_theta.data(), globals::samples);
    dp->Sub(1.0f, sin_theta.data(), sin_theta.data(), globals::samples);

    std::vector<float> sin_theta_refracted(rays.Size());
    dp->Mul(refractive_indices.data(), sin_theta.data(), sin_theta_refracted.data(), globals::samples);

    std::vector<float> cannot_refract(rays.Size());
    dp->GreaterThan(sin_theta_refracted.data(), 1.0f, cannot_refract.data(), globals::samples);

    std::vector<float> reflectance(rays.Size());
    Reflectance(cos_theta, refractive_indices, reflectance);

    // cannot_refract OR (reflectance > RandomFloat)
    std::vector<float> reflected_mask(rays.Size());
    dp->GreaterThan(reflectance.data(), RandomFloat(), reflected_mask.data(), globals::samples);
    dp->Or(cannot_refract.data(), reflected_mask.data(), reflected_mask.data(), globals::samples);

    Vec3Group reflected_directions{};
    unit_directions.Reflect(records.GetNormals(), reflected_directions);

    Vec3Group scattered_directions{};
    unit_directions.Refract(unit_directions, records.GetNormals(), refractive_indices, scattered_directions);
    // blend the reflected and refracted based on the mask
    scattered_directions.BlendVectors(reflected_directions, reflected_mask, scattered_directions);

    rays.ReplaceOrigins(records.GetHitPoints(), records.GetHitMask());
    rays.ReplaceDirections(scattered_directions, records.GetHitMask());
}

void Dielectric::Reflectance(const std::vector<float> cosine, const std::vector<float> refraction_indices,
                             std::vector<float> out) const {
    simd::DispatchBase* dp = simd::GetDispatch();

    float* r0 = m_temp_buffers.data();
    float* temp = r0 + globals::samples;
    float* cos5 = temp + globals::samples;

    // r0 = ((1 - ri) / (1 + ri))^2
    dp->Sub(1.0f, refraction_indices.data(), r0, globals::samples);
    dp->Add(refraction_indices.data(), 1.0f, temp, globals::samples);
    dp->Div(r0, temp, r0, globals::samples);
    dp->Squared(r0, r0, globals::samples);

    // (1 - cos)^5
    dp->Sub(1.0f, cosine.data(), temp, globals::samples);
    dp->Squared(temp, cos5, globals::samples);
    dp->Squared(cos5, cos5, globals::samples);
    dp->Mul(cos5, temp, cos5, globals::samples);

    // Schlick approximation: R(theta) = r0 + (1 - r0) * (1 - cos)^5
    dp->Sub(1.0f, r0, temp, globals::samples);
    dp->MulAdd(temp, cos5, r0, out.data(), globals::samples);
}

shared_ptr<Material> RandomMaterial() {
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
        { 0.4f, MakeDiffuse },
        { 0.4f, MakeMetal },
        { 0.2f, MakeGlass },
    };

    static const float total_weight{ std::accumulate(
        materials.begin(), materials.end(), 0.0f,
        [](float sum, const MaterialChoice& choice) { return sum + choice.weight; }) };

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
