#pragma once

#include <Eigen/Dense>
#include <vector>

namespace Renderer {

using Vector3 = Eigen::Vector3f;

struct DirectionalLight {
  Vector3 direction = Vector3(-0.2f, -1.0f, -0.3f).normalized();
  Vector3 color{1.0f, 1.0f, 1.0f};
};

class Lighting {
 public:
  Lighting();

  Vector3 ambient_light() const;
  DirectionalLight directional_light() const;

 private:
  DirectionalLight directional_light_;
  Vector3 ambient_light_;
};

}  // namespace Renderer
