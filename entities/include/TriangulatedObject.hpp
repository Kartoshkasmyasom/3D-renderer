#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MeshData.hpp"
#include "Transform.hpp"

#include <array>
#include <stdexcept>
#include <utility>

namespace Renderer {

class TriangulatedObject {
 public:
  TriangulatedObject() = default;
  explicit TriangulatedObject(MeshData&& data);

  ~TriangulatedObject() noexcept;

  TriangulatedObject(const TriangulatedObject& obj) = delete;
  TriangulatedObject& operator=(const TriangulatedObject& obj) = delete;
  TriangulatedObject(TriangulatedObject&& obj) noexcept;
  TriangulatedObject& operator=(TriangulatedObject&& obj) noexcept;

  void draw();
  Transform& transform();
  Vector4& color();
  const Vector4& color() const;

 private:
  void clear() noexcept;
  void upload_to_gpu(const MeshData& data);

 private:
  enum BUFFER_TYPE {
    INDEX_BUFFER = 0,
    POS_VB = 1,
    NORMAL_VB = 2,
    COLOR_VB = 3,
    NUM_BUFFERS = 4
  };

  const char POSITION_LOCATION = 0;
  const char NORMAL_LOCATION = 1;
  const char COLOR_LOCATION = 2;

  GLuint VAO_ = 0;
  std::array<GLuint, NUM_BUFFERS> buffers_{};

  MeshData mesh_data_;
  Transform transform_;
  Vector4 color_{1.0f, 0.55f, 0.57f, 1.0f};
};
}  // namespace Renderer
