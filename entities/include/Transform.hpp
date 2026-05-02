#pragma once

#include <Eigen/Geometry>

namespace Renderer {

using Vector3 = Eigen::Vector3f;
using Matrix4 = Eigen::Matrix4f;

class Transform {
 public:
  Transform() = default;

  Matrix4 matrix();

  Vector3 position{0.0f, 0.0f, 0.0f};
  Vector3 rotation{0.0f, 0.0f, 0.0f};
  Vector3 scale{1.0f, 1.0f, 1.0f};
};

}  // namespace Renderer
