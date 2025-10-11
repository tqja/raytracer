#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "Colour.h"
#include "ColourConstants.h"


class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
    ) const {
        return false;
    }

};


class Lambertian : public Material {
public:
    Lambertian(const Colour& albedo) : m_albedo(albedo) {};

    bool scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    Colour m_albedo{};
};


class Metal : public Material {
public:
    Metal(const Colour& albedo, double fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    Colour m_albedo{};
    double m_fuzz{};
};


class Dielectric : public Material {
public:
    Dielectric(double refraction_index) : m_refraction_index(refraction_index) {}

    bool scatter(const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered) const override;

private:
    double m_refraction_index;
};