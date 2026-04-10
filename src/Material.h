#pragma once

#include <memory>
#include <vector>

#include "Colour.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

class Material {
public:
    virtual ~Material() = default;

    virtual void Scatter(RayGroup&, const HitRecordGroup&, Colour&) const { return; }
};

class Lambertian : public Material {
public:
    Lambertian(const Colour& albedo) : m_albedo(albedo) {};

    void Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const override;

private:
    Colour m_albedo{};
};

class Metal : public Material {
public:
    Metal(const Colour& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    void Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const override;

private:
    Colour m_albedo{};
    float m_fuzz{};
};

class Dielectric : public Material {
public:
    Dielectric(float refraction_index) : m_refraction_index(refraction_index), m_temp_buffers(3 * globals::samples) {}

    void Scatter(RayGroup& rays, const HitRecordGroup& records, Colour& attenuation) const override;

private:
    float m_refraction_index;
    mutable std::vector<float> m_temp_buffers;

    void Reflectance(const std::vector<float> cosine, const std::vector<float> refraction_indices,
                     std::vector<float> out) const;
};

shared_ptr<Material> RandomMaterial();