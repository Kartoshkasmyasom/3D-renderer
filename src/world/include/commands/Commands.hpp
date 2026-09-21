#pragma once

#include "Requests.hpp"
#include "World.hpp"

namespace Renderer {

using ChangeColorRequest = typename RequestTraits<Action::ChangeColor>::Request;
using MoveObjectRequest = typename RequestTraits<Action::MoveObject>::Request;
using RotateObjectRequest =
    typename RequestTraits<Action::RotateObject>::Request;
using ScaleObjectRequest = typename RequestTraits<Action::ScaleObject>::Request;
using DeleteObjectRequest =
    typename RequestTraits<Action::DeleteObject>::Request;
using MoveCameraRequest = typename RequestTraits<Action::MoveForward>::Request;
using RotateCameraRequest = typename RequestTraits<Action::RotateLeft>::Request;
using SelectObjectRequest = typename RequestTraits<Action::NextObject>::Request;

template <Action action>
struct Command;

template <>
struct Command<Action::ImportObject> {
  void execute(World& world, TriangulatedObject&& object);
};

template <>
struct Command<Action::ChangeColor> {
  void execute(World& world, const ChangeColorRequest& request);
};

template <>
struct Command<Action::MoveObject> {
  void execute(World& world, const MoveObjectRequest& request);
};

template <>
struct Command<Action::RotateObject> {
  void execute(World& world, const RotateObjectRequest& request);
};

template <>
struct Command<Action::ScaleObject> {
  void execute(World& world, const ScaleObjectRequest& request);
};

template <>
struct Command<Action::DeleteObject> {
  void execute(World& world, const DeleteObjectRequest& request);
};

template <>
struct Command<Action::MoveForward> {
  void execute(World& world, const MoveCameraRequest& request);
};

template <>
struct Command<Action::MoveBackward> {
  void execute(World& world, const MoveCameraRequest& request);
};

template <>
struct Command<Action::MoveRight> {
  void execute(World& world, const MoveCameraRequest& request);
};

template <>
struct Command<Action::MoveLeft> {
  void execute(World& world, const MoveCameraRequest& request);
};

template <>
struct Command<Action::RotateLeft> {
  void execute(World& world, const RotateCameraRequest& request);
};

template <>
struct Command<Action::RotateRight> {
  void execute(World& world, const RotateCameraRequest& request);
};

template <>
struct Command<Action::RotateUp> {
  void execute(World& world, const RotateCameraRequest& request);
};

template <>
struct Command<Action::RotateDown> {
  void execute(World& world, const RotateCameraRequest& request);
};

template <>
struct Command<Action::NextObject> {
  void execute(World& world, const SelectObjectRequest& request);
};

template <>
struct Command<Action::PreviousObject> {
  void execute(World& world, const SelectObjectRequest& request);
};

}  // namespace Renderer
