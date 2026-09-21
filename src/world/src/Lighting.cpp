#include "Lighting.hpp"

namespace Renderer {

// void AmbientLight::set_color(const Vector3& color) {
//   color_ = color;
// }

Lighting::Lighting()
    : directional_light_(), ambient_light_({0.15f, 0.15f, 0.15f}) {}

Vector3 Lighting::ambient_light() const { return ambient_light_; }

DirectionalLight Lighting::directional_light() const {
  return directional_light_;
}

}  // namespace Renderer
