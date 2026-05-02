#include "ImportMesh.hpp"

namespace Renderer {
namespace {
void reserve_space(unsigned int num_vertices, unsigned int num_indices,
                   MeshData& data) {
  data.positions.reserve(num_vertices);
  data.normals.reserve(num_vertices);
  data.indices.reserve(num_indices);
}

void count_vertices_and_indices(const aiScene* pScene,
                                unsigned int& num_vertices,
                                unsigned int& num_indices, MeshData& data) {
  for (unsigned int i = 0; i < data.meshes.size(); ++i) {
    data.meshes[i].num_indices = pScene->mMeshes[i]->mNumFaces * 3;
    data.meshes[i].base_vertex = num_vertices;
    data.meshes[i].base_index = num_indices;

    num_vertices += pScene->mMeshes[i]->mNumVertices;
    num_indices += data.meshes[i].num_indices;
  }
}

void import_single_mesh(const aiMesh* mesh, MeshData& data) {
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    const aiVector3D& pos = mesh->mVertices[i];
    data.positions.push_back(Vector3(pos.x, pos.y, pos.z));

    if (mesh->HasNormals()) {
      const aiVector3D& normal = mesh->mNormals[i];
      data.normals.push_back(Vector3(normal.x, normal.y, normal.z));
    }
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    data.indices.push_back(face.mIndices[0]);
    data.indices.push_back(face.mIndices[1]);
    data.indices.push_back(face.mIndices[2]);
  }
}

void import_mesh(const aiScene* scene, MeshData& data) {
  for (unsigned int i = 0; i < data.meshes.size(); ++i) {
    const aiMesh* mesh = scene->mMeshes[i];
    import_single_mesh(mesh, data);
  }
}

void load_mesh_from_scene(const aiScene* pScene, MeshData& data) {
  data.meshes.resize(pScene->mNumMeshes);
  unsigned int num_vertices = 0;
  unsigned int num_indices = 0;

  count_vertices_and_indices(pScene, num_vertices, num_indices, data);
  reserve_space(num_vertices, num_indices, data);
  import_mesh(pScene, data);
}
}  // namespace

MeshData load_mesh_from_file(const std::string& pFile) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(
      pFile, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                 aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals |
                 aiProcess_SortByPType);

  if (nullptr == scene) {
    throw std::runtime_error(importer.GetErrorString());
  }

  MeshData result;
  load_mesh_from_scene(scene, result);
  return result;
}
}  // namespace Renderer