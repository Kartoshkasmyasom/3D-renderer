#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MeshData.hpp"

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
};
}  // namespace Renderer