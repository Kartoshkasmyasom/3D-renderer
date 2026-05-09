#pragma once

#include "Action.hpp"
#include "Commands.hpp"

namespace Renderer {

template <ConsoleAction Action>
struct ConsoleCommandTraits;

template <>
struct ConsoleCommandTraits<ConsoleAction::ImportObject> {
  using ConsoleCommand = ImportObjectCommand;
};

template <>
struct ConsoleCommandTraits<ConsoleAction::ChangeColor> {
  using ConsoleCommand = ChangeColorCommand;
};

template <>
struct ConsoleCommandTraits<ConsoleAction::MoveObject> {
  using ConsoleCommand = MoveObjectCommand;
};

template <>
struct ConsoleCommandTraits<ConsoleAction::RotateObject> {
  using ConsoleCommand = RotateObjectCommand;
};

template <>
struct ConsoleCommandTraits<ConsoleAction::ScaleObject> {
  using ConsoleCommand = ScaleObjectCommand;
};

template <>
struct ConsoleCommandTraits<ConsoleAction::DeleteObject> {
  using ConsoleCommand = DeleteObjectCommand;
};

}  // namespace Renderer
