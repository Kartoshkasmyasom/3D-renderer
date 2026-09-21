#include "object/Transform.hpp"

namespace Renderer {

Matrix4 Transform::matrix() const {
  Matrix4 translation_matrix = Matrix4::Identity();
  translation_matrix.block<3, 1>(0, 3) = position;

  Matrix4 rotation_matrix = Matrix4::Identity();
  rotation_matrix.block<3, 3>(0, 0) =
      Eigen::AngleAxisf(rotation.z(), Vector3::UnitZ()).toRotationMatrix() *
      Eigen::AngleAxisf(rotation.y(), Vector3::UnitY()).toRotationMatrix() *
      Eigen::AngleAxisf(rotation.x(), Vector3::UnitX()).toRotationMatrix();

  Matrix4 scale_matrix = Matrix4::Identity();
  scale_matrix.diagonal().head<3>() = scale;

  return translation_matrix * rotation_matrix * scale_matrix;
}

}  // namespace Renderer
