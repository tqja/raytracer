#include "Camera.h"
#include "Colour.h"
#include "Vec3.h"
#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Utility.h"
#include "ColourConstants.h"

Camera::Camera(int image_width, int image_height, double fov) : m_image_width{ image_width }, m_image_height{ image_height }, m_horizontal_fov{ fov } {
    Update();
}

Colour Camera::RayColour(const Ray& ray, const Hittable& world, int depth) const {
    if (depth >= m_max_bounce_depth) {
        return Colours::black;
    }

    HitRecord record{};
    Interval ray_interval{ 0.001, infinity };

    if (world.hit(ray, ray_interval, record)) {
        Ray scattered{};
        Colour attenuation{};

        if (record.material->Scatter(ray, record, attenuation, scattered)) {
            return attenuation * RayColour(scattered, world, depth + 1);
        }
        return Colours::black; // ray was absorbed
    }

    Vec3 unit_direction = UnitVector(ray.GetDirection());
    double blend{ (unit_direction.y() + 1.0) / 2 };

    Colour c1{ Colours::white };
    Colour c2{ 0.5, 0.7, 1.0 };

    return LerpColours(c1, c2, blend);
}

Framebuffer Camera::Render(const HittableList& world) const {
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

void Camera::Update() {
    double fov_angle{ DegToRads(m_horizontal_fov) };
    double h{ std::tan(fov_angle / 2) };

    double viewport_width = 2 * h * m_focus_distance;
    // use actual ratio instead of "ideal" ratio for calculation
    double viewport_height = viewport_width / (double(m_image_width) / m_image_height);

    Vec3 w{ UnitVector(m_camera_center - m_camera_target) };
    Vec3 u{ UnitVector(Cross(m_vup, w)) };
    Vec3 v{ Cross(w, u) };

    Vec3 viewport_u{ viewport_width * u };
    Vec3 viewport_v{ viewport_height * -v };

    m_pixel_delta_u = viewport_u / m_image_width;
    m_pixel_delta_v = viewport_v / m_image_height;

    const Vec3 vp_origin = m_camera_center
        - m_focus_distance * w
        - viewport_u / 2
        - viewport_v / 2;

    // the pixel grid is inset by 0.5px from the viewport, so must add half a pixel in both axes
    m_origin_pixel = vp_origin + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    double defocus_radius{ 
        m_focus_distance
        * std::tan(DegToRads(m_defocus_angle / 2))
    };

    m_defocus_disk_u = u * defocus_radius;
    m_defocus_disk_v = v * defocus_radius;
}

Ray Camera::GetRay(int i, int j) const {
    Vec3 offset{ SampleSquare() };
    Vec3 pixel_sample{ 
        m_origin_pixel 
        + ((i + offset.x()) * m_pixel_delta_u) 
        + ((j + offset.y()) * m_pixel_delta_v)
    };

    Vec3 ray_origin{ m_defocus_angle <= 0 ? m_camera_center : DefocusDiskSample() };
    Vec3 ray_direction{ pixel_sample - ray_origin };
    return Ray(ray_origin, ray_direction);
}

void Camera::SetCameraCenter(const Point3& look_from) {
    m_camera_center = look_from;
    Update();
}

void Camera::SetCameraTarget(const Point3& look_at) {
    m_camera_target = look_at;
    Update();
}

void Camera::SetDefocusAngle(double angle) { 
    m_defocus_angle = angle;
    Update();
}

void Camera::SetFocusDistance(double distance) { 
    m_focus_distance = distance;
    Update();
}

void Camera::SetSamplesPerPixel(int samples) {
    m_samples_per_pixel = samples;
    m_pixel_samples_scale = 1.0 / m_samples_per_pixel;
}

Vec3 Camera::SampleSquare() {
    return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
}

Point3 Camera::DefocusDiskSample() const {
    Vec3 p{ RandomInUnitDisk() };
    return m_camera_center + p[0] * m_defocus_disk_u + p[1] * m_defocus_disk_v;
}

Colour Camera::LerpColours(const Colour& c1, const Colour& c2, double blend) {
    return (1.0 - blend) * c1 + blend * c2;
}

Colour Camera::GetSampledColour(int x, int y, const Hittable& world) const {
    Colour pixel_colour{ Colours::black };

    for (int sample = 0; sample < m_samples_per_pixel; sample++) {
        Ray ray{ GetRay(x, y) };
        pixel_colour += RayColour(ray, world);
    }

    return pixel_colour * m_pixel_samples_scale;
}
