#include "Material.h"

bool Lambertian::scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    Vec3 scatter_direction = record.normal + random_unit_vector();

    if (scatter_direction.near_zero()) {
        scatter_direction = record.normal;
    }

    scattered = Ray(record.hit_point, scatter_direction);
    attenuation = m_albedo;
    return true;
}

bool Metal::scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    Vec3 reflected{ reflect(ray_in.GetDirection(), record.normal) };
    reflected = unit_vector(reflected) + m_fuzz * random_unit_vector();
    scattered = Ray(record.hit_point, reflected);
    attenuation = m_albedo;

    bool above_surface{ dot(scattered.GetDirection(), record.normal) > 0 };
    return above_surface;
}

bool Dielectric::scatter(
    const Ray& ray_in, const HitRecord& record, Colour& attenuation, Ray& scattered
) const {
    attenuation = Colours::white;
    double ri = record.front_face ? (1.0 / m_refraction_index) : m_refraction_index;

    Vec3 unit_direction = unit_vector(ray_in.GetDirection());
    Vec3 refracted = refract(unit_direction, record.normal, ri);

    scattered = Ray(record.hit_point, refracted);
    return true;  // dielectric always refracts
}
