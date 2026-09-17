#pragma once

#include <Eigen/Dense>
#include <vector>

namespace Renderer {
using Vector3 = Eigen::Vector3f;
using Vector4 = Eigen::Vector4f;

struct Mesh {
  Mesh() {
    num_indices = 0;
    base_vertex = 0;
    base_index = 0;
  }

  unsigned int num_indices;
  unsigned int base_vertex;
  unsigned int base_index;
};

struct MeshData {
  std::vector<Mesh> meshes;
  std::vector<Vector3> positions;
  std::vector<Vector3> normals;
  std::vector<unsigned int> indices;

  void clear() {
    meshes.clear();
    positions.clear();
    normals.clear();
    indices.clear();
  }
};
}  // namespace Renderer
