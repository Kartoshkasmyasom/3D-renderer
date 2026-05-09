#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "World.hpp"
#include "light/AmbientLight.hpp"
#include "light/DirectionalLight.hpp"

#include <Eigen/Dense>
#include <SFML/System/Vector2.hpp>

namespace Renderer {

class Renderer {
 public:
  Renderer(World& world, Camera& camera);
  void draw_scene(sf::Vector2u viewport_size,
                  const TriangulatedObject* highlighted_object = nullptr);

 private:
  World& world_;
  Camera& camera_;
  Shader shader_;
  DirectionalLight light_;
  AmbientLight ambient_light_;
};

}  // namespace Renderer
