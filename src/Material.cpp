#include "Material.h"

bool Lambertian::Scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    Vec3 scatter_direction = record.normal + RandomUnitVector();

    if (scatter_direction.NearZero()) {
        scatter_direction = record.normal;
    }

    scattered = Ray(record.hit_point, scatter_direction);
    attenuation = m_albedo;
    return true;
}

bool Metal::Scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    Vec3 reflected{ Reflect(ray_in.GetDirection(), record.normal) };
    reflected = UnitVector(reflected) + m_fuzz * RandomUnitVector();
    scattered = Ray(record.hit_point, reflected);
    attenuation = m_albedo;

    bool above_surface{ Dot(scattered.GetDirection(), record.normal) > 0 };
    return above_surface;
}

bool Dielectric::Scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    attenuation = Colours::white;
    double ri = record.front_face ? (1.0 / m_refraction_index) : m_refraction_index;

    Vec3 unit_direction = UnitVector(ray_in.GetDirection());
    double cos_theta = std::fmin(Dot(-unit_direction, record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Vec3 direction{};

    if (cannot_refract || Reflectance(cos_theta, ri) > RandomDouble()) {
        direction = Reflect(unit_direction, record.normal);
    }
    else {
        direction = Refract(unit_direction, record.normal, ri);
    }

    scattered = Ray(record.hit_point, direction);
    return true;  // dielectric always refracts
}

double Dielectric::Reflectance(double cosine, double refraction_index) {
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
