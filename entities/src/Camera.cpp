#include "Camera.hpp"

#include <cmath>

namespace Renderer {
Matrix4 Camera::view_matrix() const {
    Matrix4 view = Matrix4::Identity();
    view.block<1, 3>(0, 0) = camera_right_.transpose();
    view.block<1, 3>(1, 0) = camera_up_.transpose();
    view.block<1, 3>(2, 0) = direction_.transpose();
    view(0, 3) = -camera_right_.dot(position_);
    view(1, 3) = -camera_up_.dot(position_);
    view(2, 3) = -direction_.dot(position_);
    return view;
}

Matrix4 Camera::projection_matrix(float width, float height) const {
    constexpr float pi = 3.14159265358979323846f;

    const float aspect_ratio = width / height;
    const float half_fov_radians = fov_degrees_ * pi / 360.0f;
    const float tan_half_fov = std::tan(half_fov_radians);

    Matrix4 projection = Matrix4::Zero();
    projection(0, 0) = 1.0f / (tan_half_fov * aspect_ratio);
    projection(1, 1) = 1.0f / tan_half_fov;
    projection(2, 2) = -(z_far_ + z_near_) / (z_far_ - z_near_);
    projection(2, 3) = -(2.0f * z_far_ * z_near_) / (z_far_ - z_near_);
    projection(3, 2) = -1.0f;

    return projection;
}

    void Camera::set_position(const Vector3& position) {
        position_ = position;
        update_vectors();
    }

    void Camera::set_target(const Vector3& target) {
        target_ = target;
        update_vectors();
    }

    void Camera::update_vectors() {
        direction_ = (position_ - target_).normalized(); 
        camera_right_ = world_up_.cross(direction_).normalized();
        camera_up_ = direction_.cross(camera_right_).normalized();
    }
} // namespace Renderer
