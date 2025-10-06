#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "Colour.h"

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

    bool scatter(
        const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
    ) const override {
        Vec3 scatter_direction = record.normal + random_unit_vector();

        if (scatter_direction.near_zero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.hit_point, scatter_direction);
        attenuation = m_albedo;
        return true;
    }

private:
    Colour m_albedo{};
};

class Metal : public Material {
public:
    Metal(const Colour& albedo) : m_albedo(albedo) {}

    bool scatter(
        const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
    ) const override {
        Vec3 reflected{ reflect(ray_in.GetDirection(), record.normal) };
        scattered = Ray(record.hit_point, reflected);
        attenuation = m_albedo;        
        return true;
    }

private:
    Colour m_albedo{};
};