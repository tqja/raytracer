#pragma once

#include "Vec3.h"

class Camera {
public:
    Camera() {}
    Camera(int image_width, int image_height, double viewport_height);
    void update();

    const Vec3& GetCameraCenter() const { return m_camera_center; }
    const Vec3& GetOriginPixel() const { return m_origin_pixel; }
    const Vec3& GetPixelDeltaU() const { return m_pixel_delta_u; }
    const Vec3& GetPixelDeltaV() const { return m_pixel_delta_v; }

    void SetCameraCenter(const Vec3& m_camera_center);
    void SetFocalLength(double focal_length);
private:
    Vec3 m_camera_center{ 0, 0, 0 };
    double m_focal_length{ 1.0 };

    int m_image_height{};
    int m_image_width{};
    double m_viewport_height{};
    double m_viewport_width{};

    Vec3 m_pixel_delta_u{};
    Vec3 m_pixel_delta_v{};
    Vec3 m_origin_pixel{};
};