#pragma once

#include <Eigen/Dense>

namespace Renderer {

using Vector3 = Eigen::Vector3f;

class AmbientLight {
 public:
  AmbientLight() = default;
  explicit AmbientLight(const Vector3& color);

  const Vector3& color() const;
  void set_color(const Vector3& color);

 private:
  Vector3 color_{0.15f, 0.15f, 0.15f};
};

}  // namespace Renderer
