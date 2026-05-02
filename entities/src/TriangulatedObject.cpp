#include "TriangulatedObject.hpp"
#include "GlCheck.hpp"

#include <iostream>

namespace Renderer {
void TriangulatedObject::clear() noexcept {
  if (VAO_ != 0) {
    glDeleteVertexArrays(1, &VAO_);
    VAO_ = 0;
  }

  unsigned int buffer_size = buffers_.size();
  if (buffers_[0] != 0) {
    glDeleteBuffers(buffer_size, buffers_.data());
  }

  mesh_data_.clear();
}

TriangulatedObject::TriangulatedObject(MeshData&& data) : mesh_data_(std::move(data)) {
  upload_to_gpu(mesh_data_);
}

TriangulatedObject::~TriangulatedObject() {
  clear();
}

TriangulatedObject::TriangulatedObject(TriangulatedObject&& obj) noexcept {
  std::cout<<"Constructor move\n";
  VAO_ = obj.VAO_;
  buffers_ = obj.buffers_;
  mesh_data_.meshes = std::exchange(obj.mesh_data_.meshes, {});
    mesh_data_.positions = std::exchange(obj.mesh_data_.positions, {});
    mesh_data_.normals = std::exchange(obj.mesh_data_.normals, {});
    mesh_data_.indices = std::exchange(obj.mesh_data_.indices, {});
  obj.VAO_ = 0;
  obj.buffers_.fill(0);
}

TriangulatedObject& TriangulatedObject::operator=(TriangulatedObject&& obj) noexcept {
  std::cout<<"= move\n";
  if (this != &obj) {
    clear();
    VAO_ = obj.VAO_;
    buffers_ = obj.buffers_;
    mesh_data_.meshes = std::exchange(obj.mesh_data_.meshes, {});
    mesh_data_.positions = std::exchange(obj.mesh_data_.positions, {});
    mesh_data_.normals = std::exchange(obj.mesh_data_.normals, {});
    mesh_data_.indices = std::exchange(obj.mesh_data_.indices, {});
    obj.VAO_ = 0;
    obj.buffers_.fill(0);
  }
  return *this;
}

void TriangulatedObject::upload_to_gpu(const MeshData& data) {
  if (data.positions.empty() || data.indices.empty()) {
    throw std::runtime_error("Could not upload empty mesh on GPU!");
  }

  glGenVertexArrays(1, &VAO_);
  GL_CHECK(glBindVertexArray(VAO_));

  glGenBuffers(static_cast<GLsizei>(buffers_.size()), buffers_.data());

  glBindBuffer(GL_ARRAY_BUFFER, buffers_[POS_VB]);
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(sizeof(data.positions[0]) *
                                       data.positions.size()),
               &data.positions[0], GL_STATIC_DRAW));
  glEnableVertexAttribArray(POSITION_LOCATION);
  GL_CHECK(glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vector3), 0));

  if (!data.normals.empty()) {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL_VB]);
    GL_CHECK(glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(sizeof(data.normals[0]) * data.normals.size()),
        &data.normals[0], GL_STATIC_DRAW));
    glEnableVertexAttribArray(NORMAL_LOCATION);
    GL_CHECK(glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vector3), 0));
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[INDEX_BUFFER]);
  GL_CHECK(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(sizeof(data.indices[0]) * data.indices.size()),
      &data.indices[0], GL_STATIC_DRAW));

  glBindVertexArray(0);
}

void TriangulatedObject::draw() {
  GL_CHECK(glBindVertexArray(VAO_));
  for (auto& mesh : mesh_data_.meshes) {
    size_t offset = mesh.base_index * sizeof(unsigned int);
    GL_CHECK(glDrawElements(
      GL_TRIANGLES,
      mesh.num_indices, 
      GL_UNSIGNED_INT, 
      reinterpret_cast<void*>(static_cast<uintptr_t>(mesh.base_index)))
    );
  }
}
}  // namespace Renderer