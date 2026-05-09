#pragma once

#include "MeshData.hpp"

#include <string>

struct aiMesh;
struct aiScene;

namespace Renderer {

class Importer {
 public:
  MeshData load_mesh_from_file(const std::string& file) const;

 private:
  static void count_vertices_and_indices(const aiScene* scene,
                                         unsigned int& num_vertices,
                                         unsigned int& num_indices,
                                         MeshData& data);
  static void reserve_space(unsigned int num_vertices, unsigned int num_indices,
                            MeshData& data);
  static void import_single_mesh(const aiMesh* mesh, MeshData& data);
  static void import_mesh(const aiScene* scene, MeshData& data);
  static void load_mesh_from_scene(const aiScene* scene, MeshData& data);
};

}  // namespace Renderer
