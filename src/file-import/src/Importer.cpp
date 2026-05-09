#include "Importer.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <stdexcept>

namespace Renderer {

void Importer::reserve_space(unsigned int num_vertices,
                             unsigned int num_indices, MeshData& data) {
  data.positions.reserve(num_vertices);
  data.normals.reserve(num_vertices);
  data.indices.reserve(num_indices);
}

void Importer::count_vertices_and_indices(const aiScene* scene,
                                          unsigned int& num_vertices,
                                          unsigned int& num_indices,
                                          MeshData& data) {
  for (unsigned int i = 0; i < data.meshes.size(); ++i) {
    data.meshes[i].num_indices = scene->mMeshes[i]->mNumFaces * 3;
    data.meshes[i].base_vertex = num_vertices;
    data.meshes[i].base_index = num_indices;
    num_vertices += scene->mMeshes[i]->mNumVertices;
    num_indices += data.meshes[i].num_indices;
  }
}

void Importer::import_single_mesh(const aiMesh* mesh, MeshData& data) {
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    const aiVector3D& pos = mesh->mVertices[i];
    data.positions.push_back(Vector3(pos.x, pos.y, pos.z));

    if (mesh->HasNormals()) {
      const aiVector3D& normal = mesh->mNormals[i];
      data.normals.push_back(Vector3(normal.x, normal.y, normal.z));
    } else {
      data.normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    }
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    data.indices.push_back(face.mIndices[0]);
    data.indices.push_back(face.mIndices[1]);
    data.indices.push_back(face.mIndices[2]);
  }
}

void Importer::import_mesh(const aiScene* scene, MeshData& data) {
  for (unsigned int i = 0; i < data.meshes.size(); ++i) {
    import_single_mesh(scene->mMeshes[i], data);
  }
}

void Importer::load_mesh_from_scene(const aiScene* scene, MeshData& data) {
  data.meshes.resize(scene->mNumMeshes);
  unsigned int num_vertices = 0;
  unsigned int num_indices = 0;
  count_vertices_and_indices(scene, num_vertices, num_indices, data);
  reserve_space(num_vertices, num_indices, data);
  import_mesh(scene, data);
}

MeshData Importer::load_mesh_from_file(const std::string& file) const {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      file, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals |
                aiProcess_SortByPType);

  if (scene == nullptr) {
    throw std::runtime_error(importer.GetErrorString());
  }

  MeshData result;
  load_mesh_from_scene(scene, result);
  return result;
}

}  // namespace Renderer
