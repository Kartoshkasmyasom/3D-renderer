#include "Camera.hpp"

#include <algorithm>
#include <cmath>

namespace Renderer {

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kMaxPitch = 89.0f;

float to_radians(float degrees) {
  return degrees * kPi / 180.0f;
}

float clamp_pitch(float pitch_degrees) {
  return std::clamp(pitch_degrees, -kMaxPitch, kMaxPitch);
}

}  // namespace

Camera::Camera(const Vector3& position, float yaw_degrees, float pitch_degrees,
               float fov_degrees, float z_near, float z_far)
    : position_(position),
      yaw_degrees_(yaw_degrees),
      pitch_degrees_(clamp_pitch(pitch_degrees)),
      fov_degrees_(fov_degrees),
      z_near_(z_near),
      z_far_(z_far) {
  update_vectors();
}

void Camera::move_forward(float distance) {
  position_ += forward_ * distance;
}

void Camera::move_right(float distance) {
  position_ += right_ * distance;
}

void Camera::rotate_yaw(float degrees) {
  yaw_degrees_ += degrees;
  update_vectors();
}

void Camera::rotate_pitch(float degrees) {
  pitch_degrees_ = clamp_pitch(pitch_degrees_ + degrees);
  update_vectors();
}

void Camera::update_vectors() {
  const float yaw = to_radians(yaw_degrees_);
  const float pitch = to_radians(pitch_degrees_);

  forward_ = Vector3(std::cos(yaw) * std::cos(pitch), std::sin(pitch),
                     std::sin(yaw) * std::cos(pitch))
                 .normalized();
  right_ = forward_.cross(world_up_).normalized();
  up_ = right_.cross(forward_).normalized();
}

Matrix4 Camera::view_matrix() const {
  Matrix4 view = Matrix4::Identity();
  view.block<1, 3>(0, 0) = right_.transpose();
  view.block<1, 3>(1, 0) = up_.transpose();
  view.block<1, 3>(2, 0) = (-forward_).transpose();
  view(0, 3) = -right_.dot(position_);
  view(1, 3) = -up_.dot(position_);
  view(2, 3) = forward_.dot(position_);
  return view;
}

Matrix4 Camera::projection_matrix(float width, float height) const {
  const float aspect_ratio = width / height;
  const float half_fov_radians = to_radians(fov_degrees_) / 2.0f;
  const float tan_half_fov = std::tan(half_fov_radians);

  Matrix4 projection = Matrix4::Zero();
  projection(0, 0) = 1.0f / (tan_half_fov * aspect_ratio);
  projection(1, 1) = 1.0f / tan_half_fov;
  projection(2, 2) = -(z_far_ + z_near_) / (z_far_ - z_near_);
  projection(2, 3) = -(2.0f * z_far_ * z_near_) / (z_far_ - z_near_);
  projection(3, 2) = -1.0f;

  return projection;
}

const Vector3& Camera::position() const {
  return position_;
}

const Vector3& Camera::forward() const {
  return forward_;
}

const Vector3& Camera::right() const {
  return right_;
}

const Vector3& Camera::up() const {
  return up_;
}

float Camera::yaw() const {
  return yaw_degrees_;
}

float Camera::pitch() const {
  return pitch_degrees_;
}

void Camera::set_position(const Vector3& position) {
  position_ = position;
}

void Camera::set_rotation(float yaw_degrees, float pitch_degrees) {
  yaw_degrees_ = yaw_degrees;
  pitch_degrees_ = clamp_pitch(pitch_degrees);
  update_vectors();
}

}  // namespace Renderer
