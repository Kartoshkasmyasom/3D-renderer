#pragma once

#include "Camera.hpp"
#include "Importer.hpp"
#include "Renderer.hpp"
#include "World.hpp"

namespace Renderer {

struct ModelContext {
  World& world;
  Camera& camera;
  Renderer& renderer;
  Importer& importer;
};

}  // namespace Renderer
