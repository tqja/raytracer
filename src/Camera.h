#pragma once

#include "Framebuffer.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Vec3.h"

class Camera {
public:
    Camera() {}
    Camera(int image_width, int image_height, double fov);

    void Update();
    Framebuffer Render(HittableList world) const;
    Colour RayColour(const Ray& ray, const Hittable& world, int depth = 0) const;
    Point3 DefocusDiskSample() const;

    Ray  GetRay(int i, int j) const;
    void SetCameraCenter(const Point3& m_camera_center);
    void SetCameraTarget(const Point3& m_camera_center);
    void SetSamplesPerPixel(int samples);
    void SetMaxBounceDepth(int max_bounces) { m_max_bounce_depth = max_bounces; }
    void SetFOV(double fov) { m_horizontal_fov = fov; }
    void SetDefocusAngle(double angle);
    void SetFocusDistance(double distance);

private:
    Vec3 m_camera_center{ 0, 0, 0 };
    Vec3 m_camera_target{ 0, 0, -1 };
    Vec3 m_vup{ 0, 1, 0 };

    double m_defocus_angle{ 0 };
    double m_focus_distance{ 10 };
    Vec3 m_defocus_disk_u{};
    Vec3 m_defocus_disk_v{};

    int m_image_height{};
    int m_image_width{};
    double m_horizontal_fov{};

    Vec3 m_pixel_delta_u{};
    Vec3 m_pixel_delta_v{};
    Vec3 m_origin_pixel{};

    int m_samples_per_pixel{ 10 };
    double m_pixel_samples_scale{ 1.0 / m_samples_per_pixel };

    int m_max_bounce_depth{ 10 };

    Colour GetSampledColour(int x, int y, const Hittable& world) const;
    static Vec3 SampleSquare();
    static Colour LerpColours(const Colour& c1, const Colour& c2, double blend);
};