#pragma once

#include "Action.hpp"
#include "Requests.hpp"
#include "commands/CommandTraits.hpp"

namespace Renderer {

template <Action action>
struct ActionTraits {
  using Request = typename RequestTraits<action>::Request;
  using Command = typename CommandTraits<action>::Command;
};

}  // namespace Renderer
