#pragma once

#include <Eigen/Dense>

namespace Renderer {

using Matrix4 = Eigen::Matrix4f;
using Vector3 = Eigen::Vector3f;

class Camera {
 public:
  Camera(const Vector3& position = Vector3(0.0f, 0.0f, 3.0f),
         float yaw_degrees = -90.0f, float pitch_degrees = 0.0f,
         float fov_degrees = 45.0f, float z_near = 0.1f, float z_far = 100.0f,
         float movement_speed = 3.0f, float rotation_speed = 90.0f,
         const Vector3& world_up = Vector3(0.0f, 1.0f, 0.0f));

  void move_forward(float delta_time);
  void move_backward(float delta_time);
  void move_right(float delta_time);
  void move_left(float delta_time);
  void rotate_yaw(float delta_time);
  void rotate_pitch(float delta_time);
  void rotate_left(float delta_time);
  void rotate_right(float delta_time);
  void rotate_up(float delta_time);
  void rotate_down(float delta_time);
  void update_vectors();

  Matrix4 view_matrix() const;
  Matrix4 projection_matrix(float width, float height) const;

  const Vector3& position() const;
  const Vector3& forward() const;
  const Vector3& right() const;
  const Vector3& up() const;
  float yaw() const;
  float pitch() const;

  void set_position(const Vector3& position);
  void set_rotation(float yaw_degrees, float pitch_degrees);

 private:
  float movement_speed_ = 3.0f;
  float rotation_speed_ = 90.0f;
  Vector3 position_;
  float yaw_degrees_;
  float pitch_degrees_;
  Vector3 forward_;
  Vector3 right_;
  Vector3 up_;
  const Vector3 world_up_;
  float fov_degrees_;
  float z_near_;
  float z_far_;
};

}  // namespace Renderer
