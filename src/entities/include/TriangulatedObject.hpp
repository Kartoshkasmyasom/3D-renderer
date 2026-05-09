#pragma once

#include "MeshData.hpp"
#include "Transform.hpp"

#include <GL/glew.h>

#include <array>

namespace Renderer {

class TriangulatedObject {
 public:
  TriangulatedObject() = default;
  explicit TriangulatedObject(MeshData&& data);
  ~TriangulatedObject() noexcept;

  TriangulatedObject(const TriangulatedObject&) = delete;
  TriangulatedObject& operator=(const TriangulatedObject&) = delete;
  TriangulatedObject(TriangulatedObject&& obj) noexcept;
  TriangulatedObject& operator=(TriangulatedObject&& obj) noexcept;

  void draw();
  Transform& transform();
  Vector4& color();
  const Vector4& color() const;

 private:
  void clear() noexcept;
  void upload_to_gpu(const MeshData& data);

  enum BufferType {
    INDEX_BUFFER = 0,
    POS_VB = 1,
    NORMAL_VB = 2,
    COLOR_VB = 3,
    NUM_BUFFERS = 4
  };

  const char POSITION_LOCATION = 0;
  const char NORMAL_LOCATION = 1;

  GLuint vao_ = 0;
  std::array<GLuint, NUM_BUFFERS> buffers_{};
  MeshData mesh_data_;
  Transform transform_;
  Vector4 color_{1.0f, 0.55f, 0.57f, 1.0f};
};

}  // namespace Renderer
