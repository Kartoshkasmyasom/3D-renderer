#pragma once

#include <string_view>

#include "Action.hpp"

namespace Renderer {

template <Action action>
struct MessageTraits;

template <>
struct MessageTraits<Action::ImportObject> {
  static constexpr std::string_view hint =
      "Object input mode is active. Type a readable .obj file path, or "
      "\"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid file path. Type a readable .obj file path.";
};

template <>
struct MessageTraits<Action::ChangeColor> {
  static constexpr std::string_view hint =
      "Color input mode is active. Type exactly 3 RGB numbers from 0 to 255, "
      "or \"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid RGB color. Type exactly 3 whole numbers from 0 to 255.";
};

template <>
struct MessageTraits<Action::MoveObject> {
  static constexpr std::string_view hint =
      "Move input mode is active. Type a displacement vector X Y Z with "
      "components from -1000 to 1000, or \"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid displacement. Type exactly 3 components from -1000 to 1000.";
};

template <>
struct MessageTraits<Action::RotateObject> {
  static constexpr std::string_view hint =
      "Rotation input mode is active. Type rotation by X Y Z in degrees from "
      "0 to 360, or \"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid rotation. Type exactly 3 degree values from 0 to 360.";
};

template <>
struct MessageTraits<Action::ScaleObject> {
  static constexpr std::string_view hint =
      "Scale input mode is active. Type scale by X Y Z as 3 numbers from 0.1 "
      "to 10, or \"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid scale. Type exactly 3 scale values from 0.1 to 10.";
};

template <>
struct MessageTraits<Action::DeleteObject> {
  static constexpr std::string_view hint =
      "Delete input mode is active. Type y to delete the selected "
      "object, or type \"exit\" to cancel.";
  static constexpr std::string_view failure =
      "Invalid delete input. Type y to delete the selected object.";
};

}  // namespace Renderer
