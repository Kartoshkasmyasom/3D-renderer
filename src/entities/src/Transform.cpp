#include "Transform.hpp"

namespace Renderer {

Matrix4 Transform::matrix() const {
  Eigen::Affine3f translation = Eigen::Affine3f(Eigen::Translation3f(position));

  Eigen::Quaternionf rotation_quaternion =
      Eigen::AngleAxisf(rotation.z(), Vector3::UnitZ()) *
      Eigen::AngleAxisf(rotation.y(), Vector3::UnitY()) *
      Eigen::AngleAxisf(rotation.x(), Vector3::UnitX());
  rotation_quaternion.normalize();

  Eigen::Affine3f rotation_transform(rotation_quaternion);
  Eigen::Affine3f scale_transform = Eigen::Affine3f(Eigen::Scaling(scale));

  return (translation * rotation_transform * scale_transform).matrix();
}

}  // namespace Renderer
