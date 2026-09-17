#pragma once

#include <optional>
#include <vector>

#include "Action.hpp"
#include "Requests.hpp"
#include "Token.hpp"

namespace Renderer {

template <Action action>
struct Validator;

template <>
struct Validator<Action::ImportObject> {
  using Request = typename RequestTraits<Action::ImportObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<Action::ChangeColor> {
  using Request = typename RequestTraits<Action::ChangeColor>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<Action::MoveObject> {
  using Request = typename RequestTraits<Action::MoveObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<Action::RotateObject> {
  using Request = typename RequestTraits<Action::RotateObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<Action::ScaleObject> {
  using Request = typename RequestTraits<Action::ScaleObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

template <>
struct Validator<Action::DeleteObject> {
  using Request = typename RequestTraits<Action::DeleteObject>::Request;

  static std::optional<Request> validate(const std::vector<Token>& tokens);
};

}  // namespace Renderer
