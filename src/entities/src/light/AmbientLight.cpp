#include "light/AmbientLight.hpp"

namespace Renderer {

AmbientLight::AmbientLight(const Vector3& color) : color_(color) {}

const Vector3& AmbientLight::color() const {
  return color_;
}

void AmbientLight::set_color(const Vector3& color) {
  color_ = color;
}

}  // namespace Renderer
