#include "Transform.hpp"

namespace Renderer {

Matrix4 Transform::matrix() {
  Eigen::Affine3f translation =
      Eigen::Affine3f(Eigen::Translation3f(position));

  Eigen::Quaternionf q =
        Eigen::AngleAxisf(rotation.z(), Vector3::UnitZ()) *
        Eigen::AngleAxisf(rotation.y(), Vector3::UnitY()) *
        Eigen::AngleAxisf(rotation.x(), Vector3::UnitX());

    q.normalize();

    Eigen::Affine3f rotation(q);

  Eigen::Affine3f scaling = Eigen::Affine3f(Eigen::Scaling(scale));

  return (translation * rotation * scaling)
      .matrix();
}

}  // namespace Renderer
