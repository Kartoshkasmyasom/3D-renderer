#include "TriangulatedObject.hpp"

#include "GlCheck.hpp"

#include <cstdint>
#include <stdexcept>
#include <utility>

namespace Renderer {

TriangulatedObject::TriangulatedObject(MeshData&& data)
    : mesh_data_(std::move(data)) {
  upload_to_gpu(mesh_data_);
}

TriangulatedObject::~TriangulatedObject() noexcept {
  clear();
}

TriangulatedObject::TriangulatedObject(TriangulatedObject&& obj) noexcept {
  *this = std::move(obj);
}

TriangulatedObject& TriangulatedObject::operator=(
    TriangulatedObject&& obj) noexcept {
  if (this != &obj) {
    clear();
    vao_ = std::exchange(obj.vao_, 0);
    buffers_ = obj.buffers_;
    obj.buffers_.fill(0);
    mesh_data_ = std::move(obj.mesh_data_);
    transform_ = obj.transform_;
    color_ = obj.color_;
  }
  return *this;
}

void TriangulatedObject::clear() noexcept {
  if (vao_ != 0) {
    glDeleteVertexArrays(1, &vao_);
    vao_ = 0;
  }

  const GLsizei buffer_size = static_cast<GLsizei>(buffers_.size());
  if (buffers_[0] != 0) {
    glDeleteBuffers(buffer_size, buffers_.data());
    buffers_.fill(0);
  }

  mesh_data_.clear();
}

void TriangulatedObject::upload_to_gpu(const MeshData& data) {
  if (data.positions.empty() || data.indices.empty()) {
    throw std::runtime_error("Could not upload empty mesh on GPU.");
  }
  if (data.normals.size() != data.positions.size()) {
    throw std::runtime_error(
        "Could not upload mesh on GPU: normals count does not match positions "
        "count.");
  }

  glGenVertexArrays(1, &vao_);
  GL_CHECK(glBindVertexArray(vao_));

  glGenBuffers(static_cast<GLsizei>(buffers_.size()), buffers_.data());

  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers_[POS_VB]));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(sizeof(data.positions[0]) *
                                                data.positions.size()),
                        data.positions.data(), GL_STATIC_DRAW));
  GL_CHECK(glEnableVertexAttribArray(POSITION_LOCATION));
  GL_CHECK(glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE,
                                 sizeof(Vector3), nullptr));

  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL_VB]));
  GL_CHECK(glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(sizeof(data.normals[0]) * data.normals.size()),
      data.normals.data(), GL_STATIC_DRAW));
  GL_CHECK(glEnableVertexAttribArray(NORMAL_LOCATION));
  GL_CHECK(glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE,
                                 sizeof(Vector3), nullptr));

  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[INDEX_BUFFER]));
  GL_CHECK(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(sizeof(data.indices[0]) * data.indices.size()),
      data.indices.data(), GL_STATIC_DRAW));

  glBindVertexArray(0);
}

void TriangulatedObject::draw() {
  GL_CHECK(glBindVertexArray(vao_));
  for (const auto& mesh : mesh_data_.meshes) {
    const auto offset =
        static_cast<std::uintptr_t>(mesh.base_index) * sizeof(unsigned int);
    GL_CHECK(glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT,
                            reinterpret_cast<void*>(offset)));
  }
}

Transform& TriangulatedObject::transform() {
  return transform_;
}

Vector4& TriangulatedObject::color() {
  return color_;
}

const Vector4& TriangulatedObject::color() const {
  return color_;
}

}  // namespace Renderer
