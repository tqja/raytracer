#include "Camera.h"
#include "Vec3.h"

Camera::Camera(int image_width, int image_height, double viewport_height)
    : m_image_width(image_width), m_image_height(image_height), m_viewport_height(viewport_height) {

    m_viewport_width = m_viewport_height * (double(image_width) / image_height);
    update();


}

void Camera::update() {
    const Vec3 viewport_u = Vec3(m_viewport_width, 0, 0);
    const Vec3 viewport_v = Vec3(0, -m_viewport_height, 0);

    m_pixel_delta_u = viewport_u / m_image_width;
    m_pixel_delta_v = viewport_v / m_image_height;

    const Vec3 vp_origin = m_camera_center
        - Vec3(0, 0, m_focal_length)
        - viewport_u / 2 - viewport_v / 2;

    // the pixel grid is inset by 0.5px from the viewport, so must add half a pixel in both axes
    m_origin_pixel = vp_origin + (m_pixel_delta_u + m_pixel_delta_v) / 2;


}

void Camera::SetCameraCenter(const Vec3& camera_center) {
    m_camera_center = camera_center;
    update();
}


void Camera::SetFocalLength(double focal_length) {
    m_focal_length = focal_length;
    update();
};