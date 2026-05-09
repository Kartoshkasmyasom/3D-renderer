#pragma once

#include "Dialogue.hpp"
#include "Window.hpp"

namespace Renderer {

struct UIContext {
  Window& window;
  Dialogue& dialogue;
};

}  // namespace Renderer
