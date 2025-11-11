#include "Camera.h"
#include "Colour.h"
#include "Vec3.h"
#include "Framebuffer.h"
#include "Geometry.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Utility.h"
#include "ColourConstants.h"
#include "simd.h"

#include <array>

Camera::Camera(int image_width, int image_height, float fov) : m_image_width{ image_width }, m_image_height{ image_height }, m_horizontal_fov{ fov } {
    Update();
}

Colour Camera::RayColour(const Ray& ray, const Hittable& world, int depth) const {
    if (depth >= m_max_bounce_depth) {
        return Colours::black;
    }

    HitRecord record{};
    Interval ray_interval{ 0.001f, infinity };

    if (world.hit(ray, ray_interval, record)) {
        Ray scattered{};
        Colour attenuation{};

        if (record.material->Scatter(ray, record, attenuation, scattered)) {
            return attenuation * RayColour(scattered, world, depth + 1);
        }
        return Colours::black; // ray was absorbed
    }

    Vec3 unit_direction = UnitVector(ray.GetDirection());
    float blend{ (unit_direction.y() + 1.0f) / 2.0f };

    Colour c1{ Colours::white };
    Colour c2{ 0.5f, 0.7f, 1.0f };

    return LerpColours(c1, c2, blend);
}

Framebuffer Camera::Render(const HittableList& world) const {
    Framebuffer framebuffer{ m_image_width, m_image_height };

    for (int y = 0; y < m_image_height; y++) {

        std::clog << "\rScanlines remaining: " << (m_image_height - y) << std::flush;
        for (int x = 0; x < m_image_width; x += 1) {

            Point current_pixel{ static_cast<float>(x), static_cast<float>(y)};
            Colour pixel_colour{ GetSampledColour(current_pixel, world) };
            framebuffer.SetPixelColour(pixel_colour, current_pixel);
        }
    }

    return framebuffer;
}

void Camera::Update() {
    float fov_angle{ DegToRads(m_horizontal_fov) };
    float h{ std::tan(fov_angle / 2) };

    float viewport_width = 2 * h * m_focus_distance;
    // use actual ratio instead of "ideal" ratio for calculation
    float viewport_height = viewport_width / (float(m_image_width) / m_image_height);

    Vec3 w{ UnitVector(m_camera_center - m_camera_target) };
    Vec3 u{ UnitVector(Cross(m_vup, w)) };
    Vec3 v{ Cross(w, u) };

    Vec3 viewport_u{ viewport_width * u };
    Vec3 viewport_v{ viewport_height * -v };

    m_pixel_delta_u = viewport_u / static_cast<float>(m_image_width);
    m_pixel_delta_v = viewport_v / static_cast<float>(m_image_height);

    const Vec3 vp_origin = m_camera_center
        - m_focus_distance * w
        - viewport_u / 2
        - viewport_v / 2;

    // the pixel grid is inset by 0.5px from the viewport, so must add half a pixel in both axes
    m_origin_pixel = vp_origin + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    float defocus_radius{ 
        m_focus_distance
        * std::tan(DegToRads(m_defocus_angle / 2))
    };

    m_defocus_disk_u = u * defocus_radius;
    m_defocus_disk_v = v * defocus_radius;
}

void Camera::GetRayBlock(const Point& p, RayGroup& rays_out) const {
    auto* dp{ simd::GetDispatch() };


    Vec3Group offsets{};
    FillSampleSquare(offsets);
    // overwrite result into offsets
    dp->Add(offsets.x.data(), p.x, offsets.x.data(), globals::samples);
    dp->Add(offsets.y.data(), p.y, offsets.y.data(), globals::samples);

    Vec3Group pixel_samples{};

    dp->MulAddVec3(offsets, m_pixel_delta_u, m_origin_pixel, pixel_samples, globals::samples);
    dp->MulAddVec3(offsets, m_pixel_delta_v, pixel_samples, pixel_samples, globals::samples);
}

void Camera::SetCameraCenter(const Point3& look_from) {
    m_camera_center = look_from;
    Update();
}

void Camera::SetCameraTarget(const Point3& look_at) {
    m_camera_target = look_at;
    Update();
}

void Camera::SetDefocusAngle(float angle) { 
    m_defocus_angle = angle;
    Update();
}

void Camera::SetFocusDistance(float distance) { 
    m_focus_distance = distance;
    Update();
}

void Camera::SetSamplesPerPixel(int samples) {
    m_samples_per_pixel = samples;
    m_pixel_samples_scale = 1.0f / m_samples_per_pixel;
}

Vec3 Camera::SampleSquare() {
    return Vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.0f);
}

void Camera::FillSampleSquare(Vec3Group& point_group) {
    for (size_t sample = 0; sample < globals::samples; sample++) {
        point_group.x[sample] = RandomFloat() - 0.5f;
        point_group.y[sample] = RandomFloat() - 0.5f;
    }
}

Point3 Camera::DefocusDiskSample() const {
    Vec3 p{ RandomInUnitDisk() };
    return m_camera_center + p[0] * m_defocus_disk_u + p[1] * m_defocus_disk_v;
}

Colour Camera::LerpColours(const Colour& c1, const Colour& c2, float blend) {
    return (1.0f - blend) * c1 + blend * c2;
}

Colour Camera::GetSampledColour(const Point& p_pixel, const Hittable& world) const {
    Vec3Group colour_samples{};
    colour_samples.Zero();  // set all elements to black
    RayGroup ray_samples{};

    GetRayBlock(p_pixel, ray_samples);
        

    //colour_block += RayColour(ray, world); // TODO: SIMD vector addition, colour_block + RayColour()

    return Colours::black * m_pixel_samples_scale; // TODO: SIMD vector by scalar multiply, colour_block * sample scale
}
