#pragma once

#include "Action.hpp"
#include "Commands.hpp"

namespace Renderer {

template <Action action>
struct CommandTraits {
  using Command = ::Renderer::Command<action>;
};

}  // namespace Renderer
