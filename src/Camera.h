#pragma once

#include "Colour.h"
#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Camera() {}
    Camera(int image_width, int image_height, float fov);

    void Update();
    Framebuffer Render(const HittableList& world) const;
    void RayColours(const RayGroup& rays, Vec3& colour, const Hittable& world, int depth = 0) const;
    Point3 DefocusDiskSample() const;

    RayGroup GetRayBlock(const Point3 & p) const;
    void SetCameraCenter(const Point3& m_camera_center);
    void SetCameraTarget(const Point3& m_camera_center);
    void SetSamplesPerPixel(int samples);
    void SetMaxBounceDepth(int max_bounces) { m_max_bounce_depth = max_bounces; }
    void SetFOV(float fov) { m_horizontal_fov = fov; }
    void SetDefocusAngle(float angle);
    void SetFocusDistance(float distance);

private:
    Vec3 m_camera_center{ 0, 0, 0 };
    Vec3 m_camera_target{ 0, 0, -1 };
    Vec3 m_vup{ 0, 1, 0 };

    float m_defocus_angle{ 0 };
    float m_focus_distance{ 10 };
    Vec3 m_defocus_disk_u{};
    Vec3 m_defocus_disk_v{};

    int m_image_height{};
    int m_image_width{};
    float m_horizontal_fov{};

    Vec3 m_pixel_delta_u{};
    Vec3 m_pixel_delta_v{};
    Vec3 m_origin_pixel{};

    int m_samples_per_pixel{ 10 };
    float m_pixel_samples_scale{ 1.0f / m_samples_per_pixel };

    int m_max_bounce_depth{ 10 };

    static const Colour m_background_colour_1;
    static const Colour m_background_colour_2;

    Colour GetSampledColour(const Point3& p_pixel, const Hittable& world) const;
    static Vec3 SampleSquare();
    static void FillSampleSquare(Vec3Group& v);
    static void GetBlendValues(const std::vector<float> unit_directions_y, std::vector<float>& blend_values);
    void GetOrigins(Vec3Group& origins) const;
    static Vec3 LerpColours(const Colour& c1, const Colour& c2, std::vector<float>& blend);
};