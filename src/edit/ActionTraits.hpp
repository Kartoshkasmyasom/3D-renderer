#pragma once

#include "Action.hpp"
#include "Commands.hpp"
#include "ConsoleCommandTraits.hpp"
#include "MessageTraits.hpp"
#include "Requests.hpp"
#include "Validators.hpp"

namespace Renderer {

template <ConsoleAction Action>
struct ConsoleActionTraits {
  using Request = typename RequestTraits<Action>::Request;
  using Validator = ::Renderer::Validator<Action>;
  using Messages = ::Renderer::MessageTraits<Action>;
  using ConsoleCommand = typename ConsoleCommandTraits<Action>::ConsoleCommand;
};

}  // namespace Renderer
