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

Colour Camera::RayColour(const Ray& ray, const Hittable& world) const {
    HitRecord record{};
    Interval ray_interval{ 0, infinity };

    if (world.hit(ray, ray_interval, record)) {
        return (record.normal + Colour(1, 1, 1)) / 2;
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
        Vec3 row_start{ m_origin_pixel + (y * m_pixel_delta_v) };

        for (int x = 0; x < m_image_width; x++) {
            Vec3 pixel_center{ row_start + m_pixel_delta_u * x };
            Vec3 ray_direction{ pixel_center - m_camera_center };

            Ray ray{ m_camera_center, ray_direction };
            Colour ray_colour = RayColour(ray, world);

            Point current_pixel{ x, y };
            framebuffer.SetPixelColour(ray_colour, current_pixel);
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

void Camera::SetCameraCenter(const Vec3& camera_center) {
    m_camera_center = camera_center;
    UpdateOriginPixel();
}


void Camera::SetFocalLength(double focal_length) {
    m_focal_length = focal_length;
    UpdateOriginPixel();
};

Colour Camera::LerpColours(const Colour& c1, const Colour& c2, double blend) {
    return (1.0 - blend) * c1 + blend * c2;
}