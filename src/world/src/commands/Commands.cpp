#include "commands/Commands.hpp"

#include <stdexcept>
#include <utility>

#include "object/Transform.hpp"
#include "object/TriangulatedObject.hpp"

namespace Renderer {

void Command<Action::ImportObject>::execute(World& world,
                                            TriangulatedObject&& object) {
  world.add_object(std::move(object));
}

void Command<Action::ChangeColor>::execute(World& world,
                                           const ChangeColorRequest& request) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to recolor.");
  }

  auto& color = selected_object->color();
  color[0] = request.r / 255.0f;
  color[1] = request.g / 255.0f;
  color[2] = request.b / 255.0f;
}

void Command<Action::MoveObject>::execute(World& world,
                                          const MoveObjectRequest& request) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to move.");
  }

  selected_object->transform().position +=
      Vector3(request.x, request.y, request.z);
}

void Command<Action::RotateObject>::execute(
    World& world, const RotateObjectRequest& request) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to rotate.");
  }

  selected_object->transform().rotation +=
      Vector3(to_radians(request.x_degrees), to_radians(request.y_degrees),
              to_radians(request.z_degrees));
}

void Command<Action::ScaleObject>::execute(World& world,
                                           const ScaleObjectRequest& request) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to scale.");
  }

  selected_object->transform().scale = Vector3(request.x, request.y, request.z);
}

void Command<Action::DeleteObject>::execute(World& world,
                                            const DeleteObjectRequest&) {
  if (world.selected_object() == nullptr) {
    throw std::runtime_error("No selected object to delete.");
  }

  world.remove_selected_object();
}

void Command<Action::MoveForward>::execute(World& world,
                                           const MoveCameraRequest& request) {
  world.move_camera_forward(request.delta_time);
}

void Command<Action::MoveBackward>::execute(World& world,
                                            const MoveCameraRequest& request) {
  world.move_camera_backward(request.delta_time);
}

void Command<Action::MoveRight>::execute(World& world,
                                         const MoveCameraRequest& request) {
  world.move_camera_right(request.delta_time);
}

void Command<Action::MoveLeft>::execute(World& world,
                                        const MoveCameraRequest& request) {
  world.move_camera_left(request.delta_time);
}

void Command<Action::RotateLeft>::execute(World& world,
                                          const RotateCameraRequest& request) {
  world.rotate_camera_left(request.delta_time);
}

void Command<Action::RotateRight>::execute(World& world,
                                           const RotateCameraRequest& request) {
  world.rotate_camera_right(request.delta_time);
}

void Command<Action::RotateUp>::execute(World& world,
                                        const RotateCameraRequest& request) {
  world.rotate_camera_up(request.delta_time);
}

void Command<Action::RotateDown>::execute(World& world,
                                          const RotateCameraRequest& request) {
  world.rotate_camera_down(request.delta_time);
}

void Command<Action::NextObject>::execute(World& world,
                                          const SelectObjectRequest&) {
  if (!world.empty()) {
    world.select_next_object();
  }
}

void Command<Action::PreviousObject>::execute(World& world,
                                              const SelectObjectRequest&) {
  if (!world.empty()) {
    world.select_previous_object();
  }
}

}  // namespace Renderer
