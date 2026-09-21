#pragma once

#include <string>

#include "Action.hpp"

namespace Renderer {

template <auto Action>
struct RequestTraits;

template <>
struct RequestTraits<Action::ImportObject> {
  struct Request {
    std::string path;
  };
};

template <>
struct RequestTraits<Action::ChangeColor> {
  struct Request {
    float r = 255.0f;
    float g = 255.0f;
    float b = 255.0f;
  };
};

template <>
struct RequestTraits<Action::MoveObject> {
  struct Request {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
  };
};

template <>
struct RequestTraits<Action::RotateObject> {
  struct Request {
    float x_degrees = 0.0f;
    float y_degrees = 0.0f;
    float z_degrees = 0.0f;
  };
};

template <>
struct RequestTraits<Action::ScaleObject> {
  struct Request {
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
  };
};

template <>
struct RequestTraits<Action::DeleteObject> {
  struct Request {};
};

template <>
struct RequestTraits<Action::MoveForward> {
  struct Request {
    float delta_time = 0.0f;
  };
};

template <>
struct RequestTraits<Action::MoveBackward> {
  using Request = typename RequestTraits<Action::MoveForward>::Request;
};

template <>
struct RequestTraits<Action::MoveRight> {
  using Request = typename RequestTraits<Action::MoveForward>::Request;
};

template <>
struct RequestTraits<Action::MoveLeft> {
  using Request = typename RequestTraits<Action::MoveForward>::Request;
};

template <>
struct RequestTraits<Action::RotateLeft> {
  struct Request {
    float delta_time = 0.0f;
  };
};

template <>
struct RequestTraits<Action::RotateRight> {
  using Request = typename RequestTraits<Action::RotateLeft>::Request;
};

template <>
struct RequestTraits<Action::RotateUp> {
  using Request = typename RequestTraits<Action::RotateLeft>::Request;
};

template <>
struct RequestTraits<Action::RotateDown> {
  using Request = typename RequestTraits<Action::RotateLeft>::Request;
};

template <>
struct RequestTraits<Action::NextObject> {
  struct Request {};
};

template <>
struct RequestTraits<Action::PreviousObject> {
  using Request = typename RequestTraits<Action::NextObject>::Request;
};

}  // namespace Renderer
