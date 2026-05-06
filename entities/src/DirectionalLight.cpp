#include "DirectionalLight.hpp"

namespace Renderer {

DirectionalLight::DirectionalLight(const Vector3& direction,
                                   const Vector3& color)
    : direction_(direction.normalized()), color_(color) {}

const Vector3& DirectionalLight::direction() const {
  return direction_;
}

const Vector3& DirectionalLight::color() const {
  return color_;
}

void DirectionalLight::set_direction(const Vector3& direction) {
  direction_ = direction.normalized();
}

void DirectionalLight::set_color(const Vector3& color) {
  color_ = color;
}

}  // namespace Renderer
