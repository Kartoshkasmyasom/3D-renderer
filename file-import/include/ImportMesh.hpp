#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "MeshData.hpp"

#include <stdexcept>
#include <string>

namespace Renderer {

namespace {
void count_vertices_and_indices(const aiScene* pScene,
                                unsigned int& num_vertices,
                                unsigned int& num_indices, MeshData& data);
void reserve_space(unsigned int num_vertices, unsigned int num_indices,
                   MeshData& data);
void import_single_mesh(const aiMesh* mesh, MeshData& data);
void import_mesh(const aiScene* scene, MeshData& data);
void load_mesh_from_scene(const aiScene* pScene, MeshData& data);
}  // namespace

MeshData load_mesh_from_file(const std::string& pFile);

}  // namespace Renderer