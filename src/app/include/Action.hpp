#pragma once

namespace Renderer {

enum class Action {
  ImportObject,
  ChangeColor,
  MoveObject,
  RotateObject,
  ScaleObject,
  DeleteObject,
  None,
  MoveForward,
  MoveBackward,
  MoveRight,
  MoveLeft,
  RotateLeft,
  RotateRight,
  RotateUp,
  RotateDown,
  Edit,
  Back,
  NextObject,
  PreviousObject,
};

}  // namespace Renderer
