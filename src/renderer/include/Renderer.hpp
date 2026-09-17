#pragma once

#include <Eigen/Dense>
#include <SFML/System/Vector2.hpp>

#include "Shader.hpp"
#include "World.hpp"

namespace Renderer {

class Renderer {
 public:
  explicit Renderer(const World& world);
  void draw_scene(sf::Vector2u viewport_size,
                  const TriangulatedObject* highlighted_object = nullptr);

 private:
  const World& world_;
  Shader shader_;
};

}  // namespace Renderer
