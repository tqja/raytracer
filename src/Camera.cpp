#include "Camera.h"
#include "Colour.h"
#include "Vec3.h"
#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Utility.h"

Camera::Camera(int image_width, int image_height) : m_image_width{ image_width }, m_image_height{ image_height } {
    m_viewport_height = 2.0;
    // use actual ratio instead of "ideal" ratio for calculation
    m_viewport_width = m_viewport_height * (double(m_image_width) / m_image_height);

    m_viewport_u = Vec3(m_viewport_width, 0, 0);
    m_viewport_v = Vec3(0, -m_viewport_height, 0);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    UpdateOriginPixel();
}

Colour Camera::RayColour(const Ray& ray, const Hittable& world, int depth) const {
    if (depth >= m_max_bounce_depth) {
        return Colour(0, 0, 0);
    }

    HitRecord record{};
    Interval ray_interval{ 0.001, infinity };

    if (world.hit(ray, ray_interval, record)) {
        Vec3 direction{ record.normal + random_unit_vector() };
        Ray bounce_ray{ record.hit_point, direction };
        return RayColour(bounce_ray, world, depth + 1) * 0.5;
    }

    Vec3 unit_direction = unit_vector(ray.GetDirection());
    double blend{ (unit_direction.y() + 1.0) / 2 };

    Colour c1{ 1.0, 1.0, 1.0 };
    Colour c2{ 0.5, 0.7, 1.0 };

    return LerpColours(c1, c2, blend);
}

Framebuffer Camera::Render(HittableList world) const {
    Framebuffer framebuffer{ m_image_width, m_image_height };

    for (int y = 0; y < m_image_height; y++) {
        std::clog << "\rScanlines remaining: " << (m_image_height - y) << ' ' << std::flush;
        for (int x = 0; x < m_image_width; x++) {
            Point current_pixel{ x, y };
            Colour pixel_colour{ GetSampledColour(x, y, world) };
            framebuffer.SetPixelColour(pixel_colour, current_pixel);
        }
    }

    return framebuffer;
}

void Camera::UpdateOriginPixel() {
    const Vec3 vp_origin = m_camera_center
        - Vec3(0, 0, m_focal_length)
        - m_viewport_u / 2 - m_viewport_v / 2;

    // the pixel grid is inset by 0.5px from the viewport, so must add half a pixel in both axes
    m_origin_pixel = vp_origin + (m_pixel_delta_u + m_pixel_delta_v) / 2;
}

Ray Camera::GetRay(int i, int j) const {
    Vec3 offset{ SampleSquare() };
    Vec3 pixel_sample{ 
        m_origin_pixel 
        + ((i + offset.x()) * m_pixel_delta_u) 
        + ((j + offset.y()) * m_pixel_delta_v)
    };

    Vec3 ray_direction{ pixel_sample - m_camera_center };
    return Ray(m_camera_center, ray_direction);
}

void Camera::SetCameraCenter(const Vec3& camera_center) {
    m_camera_center = camera_center;
    UpdateOriginPixel();
}


void Camera::SetFocalLength(double focal_length) {
    m_focal_length = focal_length;
    UpdateOriginPixel();
}

void Camera::SetSamplesPerPixel(int samples) {
    m_samples_per_pixel = samples;
    m_pixel_samples_scale = 1.0 / m_samples_per_pixel;
}

Vec3 Camera::SampleSquare() {
    return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
}

Colour Camera::LerpColours(const Colour& c1, const Colour& c2, double blend) {
    return (1.0 - blend) * c1 + blend * c2;
}

Colour Camera::GetSampledColour(int x, int y, const Hittable& world) const {
    Colour pixel_colour{ 0, 0, 0 };

    for (int sample = 0; sample < m_samples_per_pixel; sample++) {
        Ray ray{ GetRay(x, y) };
        pixel_colour += RayColour(ray, world);
    }

    return pixel_colour * m_pixel_samples_scale;
}
