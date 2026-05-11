#pragma once

#include "Camera.hpp"
#include "Dialogue.hpp"
#include "EditSubsystem.hpp"
#include "Importer.hpp"
#include "ModelContext.hpp"
#include "Renderer.hpp"
#include "UIContext.hpp"
#include "Window.hpp"
#include "World.hpp"

#include <SFML/Window/Event.hpp>

#include <string_view>

namespace Renderer {

enum class AppState {
  Rendering,
  Edit,
};

enum class KeyboardAction {
  None,
  Import,
  Edit,
  Back,
  NextObject,
  PreviousObject,
  Color,
  Position,
  Scale,
  Rotation,
  Delete,
};

class App {
 public:
  static const std::string help_rendering =
      "Rendering mode is active.\n"
      "Press E to enter edit mode.";

  static const std::string help_edit =
      "Edit mode is active.\n"
      "Use Left/Right to select objects. Press + to import, C to change color, "
      "P to change position, S to change scale, R to change rotation, Delete "
      "to delete, B to return to rendering mode.";

  App();

  int run();

 private:
  KeyboardAction action_from_event(const sf::Event& event) const;
  void switch_state(AppState next_state);
  void update_camera(float delta_time);
  void render_frame(const TriangulatedObject* highlighted_object = nullptr);
  void print_frame_time(float delta_time, float& accumulator) const;

  template <ConsoleAction Action>
  void handle_console_command();

  ModelContext model_context_;
  UIContext ui_context_;
  AppState state_ = AppState::Rendering;
};

}  // namespace Renderer
