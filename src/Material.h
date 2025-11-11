#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "Colour.h"
#include "ColourConstants.h"


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