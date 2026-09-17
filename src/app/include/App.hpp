#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <set>
#include <string_view>

#include "ActionTraits.hpp"
#include "Dialogue.hpp"
#include "Importer.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "World.hpp"

namespace Renderer {

enum class AppState {
  Rendering,
  Edit,
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
  void switch_state(AppState next_state);
  void render_frame(const TriangulatedObject* highlighted_object = nullptr);

  template <Action action>
  void handle_console_command();

  template <Action action>
  void handle_keyboard_command();

  Window window_;
  Importer importer_;
  World world_;
  Renderer renderer_;
  Dialogue dialogue_;
  AppState state_ = AppState::Rendering;
  std::set<sf::Keyboard::Key> pressed_keys_;
  float delta_time_ = 0.0f;
};

}  // namespace Renderer
