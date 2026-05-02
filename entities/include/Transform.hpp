#pragma once

#include <Eigen/Geometry>

namespace Renderer {

using Vector3 = Eigen::Vector3f;
using Matrix4 = Eigen::Matrix4f;

constexpr float to_radians(float degrees) {
  return degrees * 3.14159265358979323846f / 180.0f;
}

class Transform {
 public:
  Transform() = default;

  Matrix4 matrix() const;

  Vector3 position{0.0f, 0.0f, 0.0f};
  Vector3 rotation{0.0f, 0.0f, 0.0f};
  Vector3 scale{1.0f, 1.0f, 1.0f};
};

}  // namespace Renderer
