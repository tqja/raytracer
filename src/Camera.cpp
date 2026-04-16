#include "Camera.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "Colour.h"
#include "ColourConstants.h"
#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Interval.h"
#include "Material.h"
#include "Ray.h"
#include "Utility.h"
#include "Vec3.h"
#include "simd/simd.h"

const Colour Camera::m_background_colour_1 = Colours::white;
const Colour Camera::m_background_colour_2 = { 0.5f, 0.7f, 1.0f };

Camera::Camera(int image_width, int image_height, float fov)
    : m_image_width{ image_width }, m_image_height{ image_height }, m_horizontal_fov{ fov } {
    Update();
}

Vec3 Camera::RayColours(RayGroup& rays, const Hittable& world) const {
    simd::DispatchBase* dp{ simd::GetDispatch() };
    const size_t ray_count{ rays.Size() };
    std::vector<float> no_hit_mask(ray_count);
    HitRecordGroup records{ ray_count };

    Vec3Group colours{ std::vector<float>(ray_count, 1.0f), std::vector<float>(ray_count, 1.0f),
                       std::vector<float>(ray_count, 1.0f) };

    for (int i = 0; i < m_max_bounce_depth; i++) {
        IntervalGroup ray_t{ 0.001f, infinity, ray_count };
        world.hit(rays, ray_t, records);

        std::vector<float> new_hit_mask(ray_count);
        std::vector<float> zero(ray_count);
        dp->GreaterThan(records.GetTValues().data(), zero.data(), new_hit_mask.data(), ray_count);
        std::vector<float> new_no_hit_mask(ray_count);
        dp->Not(new_hit_mask.data(), new_no_hit_mask.data(), ray_count);

        // maintain rays that still haven't hit within never_hit_mask
        dp->Or(no_hit_mask.data(), new_no_hit_mask.data(), ray_count);

        bool no_rays_hit{ dp->NoBitsSet(new_hit_mask.data(), ray_count) };
        if (no_rays_hit) {
            // update colours with bg colour for nohits

            std::vector<float> blend(ray_count);
            // Vec3Group unit_directions{};
            // rays.GetDirections().UnitVectors(unit_directions);
            dp->Add(rays.GetDirections().y().data(), 1.0f, blend.data(), ray_count);
            dp->Mul(blend.data(), 0.5f, ray_count);

            Vec3Group background_colours{};
            LerpColours(m_background_colour_1, m_background_colour_2, blend, background_colours);
            colours.Mul(background_colours);
            break;
        }

        Colour attenuation{};
        records.GetMaterial()->Scatter(rays, records, attenuation);

        // then update colours with mat attenuation for new hits and continue loop
        Vec3Group attenuation_group{};
        attenuation_group.Broadcast(attenuation);
        attenuation_group.BlendScalar(1.0f, no_hit_mask);
        colours.Mul(attenuation_group);
    }
    return colours.Reduce();
}

Framebuffer Camera::Render(const HittableList& world) const {
    Framebuffer framebuffer{ m_image_width, m_image_height };

    for (int y = 0; y < m_image_height; y++) {
        std::clog << "\rScanlines remaining: " << (m_image_height - y) << '\n';
        for (int x = 0; x < m_image_width; x += 1) {
            std::clog << "\rCols remaining: " << (m_image_width - x) << std::flush;
            Point3 current_pixel{ static_cast<float>(x), static_cast<float>(y), 0.0f };
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

    const Vec3 vp_origin = m_camera_center - m_focus_distance * w - viewport_u / 2 - viewport_v / 2;

    // the pixel grid is inset by 0.5px from the viewport, so must add half a pixel in both axes
    m_origin_pixel = vp_origin + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    float defocus_radius{ m_focus_distance * std::tan(DegToRads(m_defocus_angle / 2)) };

    m_defocus_disk_u = u * defocus_radius;
    m_defocus_disk_v = v * defocus_radius;
}

RayGroup Camera::GetRayBlock(const Point3& p) const {
    Vec3Group offsets{};
    FillSampleSquare(offsets);
    offsets += p;

    // Compute: pixel_samples = origin_pixel + (offsets.x * pixel_delta_u) + (offsets.y * pixel_delta_v)
    Vec3Group pixel_samples{};

    // origin_pixel + (offsets.x * pixel_delta_u), store result in delta_u
    Vec3Group delta_u{};
    delta_u.Broadcast(m_pixel_delta_u);
    delta_u.MulAdd(offsets.x(), m_origin_pixel);

    // stored result + (offsets.y * pixel_delta_v), store result in delta_v
    Vec3Group delta_v{};
    delta_v.Broadcast(m_pixel_delta_v);
    delta_v.MulAdd(offsets.y(), delta_u);

    Vec3Group ray_origins{};
    GetOrigins(ray_origins);

    Vec3Group ray_directions{};
    delta_v.Sub(ray_origins, ray_directions);
    return RayGroup{ ray_origins, ray_directions };
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

Vec3 Camera::SampleSquare() { return Vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.0f); }

void Camera::FillSampleSquare(Vec3Group& point_group) {
    for (size_t sample = 0; sample < globals::samples; sample++) {
        point_group.SetXAtIndex(sample, RandomFloat() - 0.5f);
        point_group.SetYAtIndex(sample, RandomFloat() - 0.5f);
    }
}

void Camera::GetBlendValues(const std::vector<float> unit_directions_y, std::vector<float>& blend_values) {
    simd::DispatchBase* dp{ simd::GetDispatch() };
    dp->Add(unit_directions_y.data(), 1.0f, blend_values.data(), globals::samples);
    dp->Mul(blend_values.data(), 0.5f, blend_values.data(), globals::samples);
}

Point3 Camera::DefocusDiskSample() const {
    Vec3 p{ RandomInUnitDisk() };
    return m_camera_center + p[0] * m_defocus_disk_u + p[1] * m_defocus_disk_v;
}

void Camera::GetOrigins(Vec3Group& origins) const {
    if (m_defocus_angle <= 0) {
        origins.Broadcast(m_camera_center);
        return;
    }

    for (size_t sample = 0; sample < globals::samples; sample++) {
        auto d{ DefocusDiskSample() };
        origins.SetElement(sample, d);
    }
}

void Camera::LerpColours(const Colour& c1, const Colour& c2, const std::vector<float>& blend, Vec3Group& out) {
    simd::DispatchBase* dp{ simd::GetDispatch() };

    // c2_blend = c2 * blend
    Vec3Group c2_group{};
    Vec3Group c2_blend{};
    c2_group.Broadcast(c2);
    c2_group.Mul(blend, c2_blend);

    // inverted_blend = (1.0f - blend)
    std::vector<float> inverted_blend{ blend };
    dp->Sub(1.0f, blend.data(), inverted_blend.data(), globals::samples);

    // c1 * inverted_blend + c2_blend
    Vec3Group c1_group{};
    c1_group.Broadcast(c1);
    c1_group.MulAdd(inverted_blend, c2_blend, out);
}

Colour Camera::GetSampledColour(const Point3& p_pixel, const Hittable& world) const {
    RayGroup ray_samples{ GetRayBlock(p_pixel) };
    Colour colour{ RayColours(ray_samples, world) };
    colour = colour * m_pixel_samples_scale;
    return colour;
}

static void update_colours(Vec3Group& colours, const Vec3Group& new_colours, float* mask) {
    Vec3Group new_colour_group{};
}
