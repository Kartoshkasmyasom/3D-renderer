#pragma once

#include <Eigen/Dense>

namespace Renderer {

using Vector3 = Eigen::Vector3f;
using Matrix4 = Eigen::Matrix4f;

class Camera {
 public:
  Camera(const Vector3& position = Vector3(0.0f, 0.0f, 3.0f),
         float yaw_degrees = -90.0f, float pitch_degrees = 0.0f,
         float fov_degrees = 45.0f, float z_near = 0.1f,
         float z_far = 100.0f);

  void move_forward(float distance);
  void move_right(float distance);
  void rotate_yaw(float degrees);
  void rotate_pitch(float degrees);
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
  Vector3 position_;
  float yaw_degrees_;
  float pitch_degrees_;

  Vector3 forward_;
  Vector3 right_;
  Vector3 up_;
  Vector3 world_up_{0.0f, 1.0f, 0.0f};

  float fov_degrees_;
  float z_near_;
  float z_far_;
};

}  // namespace Renderer
