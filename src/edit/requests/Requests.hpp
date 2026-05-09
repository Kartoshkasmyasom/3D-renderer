#pragma once

#include "Action.hpp"

#include <string>

namespace Renderer {

template <ConsoleAction Action>
struct RequestTraits;

template <>
struct RequestTraits<ConsoleAction::ImportObject> {
  struct Request {
    std::string path;
  };
};

template <>
struct RequestTraits<ConsoleAction::ChangeColor> {
  struct Request {
    float r = 255.0f;
    float g = 255.0f;
    float b = 255.0f;
  };
};

template <>
struct RequestTraits<ConsoleAction::MoveObject> {
  struct Request {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
  };
};

template <>
struct RequestTraits<ConsoleAction::RotateObject> {
  struct Request {
    float x_degrees = 0.0f;
    float y_degrees = 0.0f;
    float z_degrees = 0.0f;
  };
};

template <>
struct RequestTraits<ConsoleAction::ScaleObject> {
  struct Request {
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
  };
};

template <>
struct RequestTraits<ConsoleAction::DeleteObject> {
  struct Request {};
};

}  // namespace Renderer
