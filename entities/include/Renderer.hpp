#pragma once

#include "AmbientLight.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "World.hpp"

#include <Eigen/Dense>

namespace Renderer {

class Renderer {
 public:
  Renderer(Window& window, World& world, Camera& camera);
  void draw_scene(const TriangulatedObject* highlighted_object = nullptr);

 private:
  Window& window_;
  World& world_;
  Camera& camera_;
  Shader shader_;
  DirectionalLight light_;
  AmbientLight ambient_light_;
};

}  // namespace Renderer
