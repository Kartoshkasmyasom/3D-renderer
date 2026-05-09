#pragma once

#include <Eigen/Dense>

namespace Renderer {

using Vector3 = Eigen::Vector3f;

class DirectionalLight {
 public:
  DirectionalLight(const Vector3& direction = Vector3(-0.2f, -1.0f, -0.3f),
                   const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));

  const Vector3& direction() const;
  const Vector3& color() const;

  void set_direction(const Vector3& direction);
  void set_color(const Vector3& color);

 private:
  Vector3 direction_;
  Vector3 color_;
};

}  // namespace Renderer
