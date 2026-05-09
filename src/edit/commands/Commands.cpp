#include "Commands.hpp"

#include "Transform.hpp"
#include "TriangulatedObject.hpp"

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace Renderer {

void ImportObjectCommand::execute(ModelContext& context,
                                  const ImportObjectRequest& request) {
  try {
    auto mesh = context.importer.load_mesh_from_file(request.path);
    TriangulatedObject object(std::move(mesh));
    context.world.add_object(std::move(object));
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to import object: ") +
                             e.what());
  }
}

void ChangeColorCommand::execute(ModelContext& context,
                                 const ChangeColorRequest& request) {
  auto* selected_object = context.world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to recolor.");
  }

  auto& color = selected_object->color();
  color[0] = request.r / 255.0f;
  color[1] = request.g / 255.0f;
  color[2] = request.b / 255.0f;
}

void MoveObjectCommand::execute(ModelContext& context,
                                const MoveObjectRequest& request) {
  auto* selected_object = context.world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to move.");
  }

  selected_object->transform().position =
      Vector3(request.x, request.y, request.z);
}

void RotateObjectCommand::execute(ModelContext& context,
                                  const RotateObjectRequest& request) {
  auto* selected_object = context.world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to rotate.");
  }

  selected_object->transform().rotation =
      Vector3(to_radians(request.x_degrees), to_radians(request.y_degrees),
              to_radians(request.z_degrees));
}

void ScaleObjectCommand::execute(ModelContext& context,
                                 const ScaleObjectRequest& request) {
  auto* selected_object = context.world.selected_object();
  if (selected_object == nullptr) {
    throw std::runtime_error("No selected object to scale.");
  }

  selected_object->transform().scale = Vector3(request.x, request.y, request.z);
}

void DeleteObjectCommand::execute(ModelContext& context,
                                  const DeleteObjectRequest&) {
  if (context.world.selected_object() == nullptr) {
    throw std::runtime_error("No selected object to delete.");
  }

  context.world.remove_selected_object();
}

}  // namespace Renderer
