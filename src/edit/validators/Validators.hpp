#pragma once

#include "Action.hpp"
#include "Requests.hpp"
#include "Token.hpp"

#include <optional>
#include <vector>

namespace Renderer {

template <ConsoleAction Action>
struct Validator;

template <>
struct Validator<ConsoleAction::ImportObject> {
  using Request = typename RequestTraits<ConsoleAction::ImportObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<ConsoleAction::ChangeColor> {
  using Request = typename RequestTraits<ConsoleAction::ChangeColor>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<ConsoleAction::MoveObject> {
  using Request = typename RequestTraits<ConsoleAction::MoveObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<ConsoleAction::RotateObject> {
  using Request = typename RequestTraits<ConsoleAction::RotateObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<ConsoleAction::ScaleObject> {
  using Request = typename RequestTraits<ConsoleAction::ScaleObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<ConsoleAction::DeleteObject> {
  using Request = typename RequestTraits<ConsoleAction::DeleteObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

}  // namespace Renderer
