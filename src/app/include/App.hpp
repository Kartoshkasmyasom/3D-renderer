#pragma once

#include "ActionTraits.hpp"
#include "Camera.hpp"
#include "Dialogue.hpp"
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
  static constexpr std::string_view help_rendering =
      "Rendering mode is active.\n"
      "Use W/S to move forward/backward, A/D to move left/right, and arrow "
      "keys to rotate the camera.\n"
      "Press E to enter edit mode.";

  static constexpr std::string_view help_edit =
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

  Window window_;
  World world_;
  Camera camera_;
  Renderer renderer_;
  Importer importer_;
  Dialogue dialogue_;
  ModelContext model_context_;
  UIContext ui_context_;
  AppState state_ = AppState::Rendering;
};

}  // namespace Renderer
