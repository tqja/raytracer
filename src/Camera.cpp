#include "Camera.h"
#include "Colour.h"
#include "Vec3.h"
#include "ColourConstants.h"
#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Utility.h"
#include "simd.h"

const Colour Camera::m_background_colour_1 = Colours::white;
const Colour Camera::m_background_colour_2 = { 0.5f, 0.7f, 1.0f };

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

    // must pass blend array and generate colour for each ray in nohit mask.
    // accumulate each colour in the output variable
    colour += LerpColours(m_background_colour_1, m_background_colour_2, blend_values);
}

Framebuffer Camera::Render(const HittableList& world) const {
    Framebuffer framebuffer{ m_image_width, m_image_height };

    for (int y = 0; y < m_image_height; y++) {

        std::clog << "\rScanlines remaining: " << (m_image_height - y) << std::flush;
        for (int x = 0; x < m_image_width; x += 1) {

            Point3 current_pixel{ static_cast<float>(x), static_cast<float>(y), 0.0f};
            Colour pixel_colours{ GetSampledColour(current_pixel, world) };
            framebuffer.SetPixelColour(pixel_colours, current_pixel);
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

RayGroup Camera::GetRayBlock(const Point3& p, RayGroup& rays_out) const {
    auto* dp{ simd::GetDispatch() };


    Vec3Group offsets{};
    FillSampleSquare(offsets);
    offsets += p;

    Vec3Group pixel_samples{};

    offsets.MulAdd(m_pixel_delta_u, m_origin_pixel, pixel_samples);
    offsets.MulAdd(m_pixel_delta_v, pixel_samples, pixel_samples);

    Vec3Group ray_origins{};
    GetOrigins(ray_origins);    

    Vec3Group ray_directions{};
    pixel_samples.Sub(ray_origins, ray_directions);
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

Vec3 Camera::SampleSquare() {
    return Vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.0f);
}

void Camera::FillSampleSquare(Vec3Group& point_group) {
    for (size_t sample = 0; sample < globals::samples; sample++) {
        point_group.SetX(sample, RandomFloat() - 0.5f);
        point_group.SetY(sample, RandomFloat() - 0.5f);
    }
    }
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

Vec3 Camera::LerpColours(const Colour& c1, const Colour& c2, std::vector<float>& blend) {
    simd::DispatchBase* dp{ simd::GetDispatch() };
    Vec3Group blend_vec{blend, blend, blend};  // TODO: find a way to perform the operations without constructing Vec3Group
    
    Vec3Group c2_blend{};
    c2_blend.Broadcast(c2);
    c2_blend.Mul(blend_vec, c2_blend);

    dp->Sub(1.0f, blend.data(), blend.data(), globals::samples);
    blend_vec = { blend, blend, blend };

    Vec3Group c1_blend{};
    c1_blend.Broadcast(c1);
    // store result in c1_blend to avoid creating a new Vec3Group
    c1_blend.MulAdd(blend_vec, c2_blend, c1_blend);

    return c1_blend.Reduce();    
}

Colour Camera::GetSampledColour(const Point3& p_pixel, const Hittable& world) const {
    Vec3 accumulated_colours{};
    RayGroup ray_samples{ GetRayBlock(p_pixel) };
    RayColours(ray_samples, accumulated_colours, world);

    return accumulated_colours * m_pixel_samples_scale;
}
