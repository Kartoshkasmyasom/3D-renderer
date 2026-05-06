#include "ImportMesh.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "TriangulatedObject.hpp"
#include "Window.hpp"
#include "World.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

enum class AppState {
  Rendering,
  ObjectInput,
  Edit,
  ColorInput,
  PositionInput,
  ScaleInput,
  RotationInput,
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
};

using RgbColor = std::array<int, 3>;
using VectorInput = std::array<float, 3>;

void update_camera(Renderer::Camera& camera, const Renderer::Window& window,
                   float delta_time) {
  constexpr float movement_speed = 3.0f;
  constexpr float rotation_speed = 90.0f;

  const float movement = movement_speed * delta_time;
  const float rotation = rotation_speed * delta_time;

  if (window.is_key_pressed(sf::Keyboard::Key::W)) {
    camera.move_forward(movement);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::S)) {
    camera.move_forward(-movement);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::D)) {
    camera.move_right(movement);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::A)) {
    camera.move_right(-movement);
  }

  if (window.is_key_pressed(sf::Keyboard::Key::Left)) {
    camera.rotate_yaw(-rotation);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::Right)) {
    camera.rotate_yaw(rotation);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::Up)) {
    camera.rotate_pitch(rotation);
  }
  if (window.is_key_pressed(sf::Keyboard::Key::Down)) {
    camera.rotate_pitch(-rotation);
  }
}

void switch_state(AppState& state, AppState next_state) {
  state = next_state;

  if (state == AppState::ObjectInput) {
    std::cout << "\nRendering paused. Object input mode is active.\n";
    std::cout << "Type \"exit\" at any step to return to rendering mode.\n\n";
  } else if (state == AppState::Edit) {
    std::cout << "\nEdit mode is active.\n\n";
    std::cout << "Use Left/Right to select objects. Press C to change color. "
                 "Press P to change position. Press S to change scale. "
                 "Press R to change rotation. Press B to return.\n";
  } else if (state == AppState::ColorInput) {
    std::cout << "\nColor input mode is active.\n\n";
    std::cout << "Type exactly 3 RGB numbers from 0 to 255, or \"exit\" "
                 "to return to edit mode.\n";
  } else if (state == AppState::PositionInput) {
    std::cout << "\nPosition input mode is active.\n\n";
    std::cout << "Type exactly 3 coordinates from -1000 to 1000, or \"exit\" "
                 "to return to edit mode.\n";
  } else if (state == AppState::ScaleInput) {
    std::cout << "\nScale input mode is active.\n\n";
    std::cout << "Type scale by X Y Z as 3 numbers from 0.1 to 10, or "
                 "\"exit\" to return to edit mode.\n";
  } else if (state == AppState::RotationInput) {
    std::cout << "\nRotation input mode is active.\n\n";
    std::cout << "Type rotation by X Y Z in degrees from 0 to 360, or "
                 "\"exit\" to return to edit mode.\n";
  } else {
    std::cout << "\nRendering resumed.\n\n";
  }
}

std::string trim(const std::string& value) {
  const auto begin =
      std::find_if_not(value.begin(), value.end(), [](unsigned char c) {
        return std::isspace(c);
      });
  const auto end =
      std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) {
        return std::isspace(c);
      }).base();

  if (begin >= end) {
    return "";
  }
  return std::string(begin, end);
}

bool is_exit_command(const std::string& input) {
  std::string value = trim(input);
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char c) {
                   return static_cast<char>(std::tolower(c));
                 });
  return value == "exit";
}

bool parse_rgb_color(const std::string& input, RgbColor& color,
                     std::string& error_message) {
  std::istringstream stream(input);
  std::string token;

  for (std::size_t index = 0; index < color.size(); ++index) {
    if (!(stream >> token)) {
      error_message = "enter exactly 3 numbers";
      return false;
    }

    if (token.empty() ||
        !std::all_of(token.begin(), token.end(),
                     [](unsigned char c) { return std::isdigit(c) != 0; })) {
      error_message = "RGB values must be whole numbers from 0 to 255";
      return false;
    }

    int value = 0;
    for (char digit : token) {
      value = value * 10 + digit - '0';
      if (value > 255) {
        error_message = "RGB values must be in the range from 0 to 255";
        return false;
      }
    }

    color[index] = value;
  }

  if (stream >> token) {
    error_message = "enter exactly 3 numbers";
    return false;
  }

  return true;
}

bool parse_float_triplet(const std::string& input, VectorInput& values,
                         float min_value, float max_value,
                         const std::string& value_name,
                         std::string& error_message) {
  std::istringstream stream(input);
  std::string token;

  for (std::size_t index = 0; index < values.size(); ++index) {
    if (!(stream >> token)) {
      error_message = "enter exactly 3 numbers";
      return false;
    }

    std::istringstream token_stream(token);
    float value = 0.0f;
    token_stream >> value;

    if (!token_stream || !token_stream.eof()) {
      error_message = "coordinates must be numbers";
      return false;
    }

    if (!std::isfinite(value)) {
      error_message = value_name + " must be finite numbers";
      return false;
    }

    if (value < min_value || value > max_value) {
      std::ostringstream message;
      message << value_name << " must be in the range from " << min_value
              << " to " << max_value;
      error_message = message.str();
      return false;
    }

    values[index] = value;
  }

  if (stream >> token) {
    error_message = "enter exactly 3 numbers";
    return false;
  }

  return true;
}

bool parse_object_position(const std::string& input, VectorInput& position,
                           std::string& error_message) {
  return parse_float_triplet(input, position, -1000.0f, 1000.0f, "coordinates",
                             error_message);
}

bool parse_object_scale(const std::string& input, VectorInput& scale,
                        std::string& error_message) {
  return parse_float_triplet(input, scale, 0.1f, 10.0f, "scale values",
                             error_message);
}

bool parse_object_rotation(const std::string& input, VectorInput& rotation,
                           std::string& error_message) {
  return parse_float_triplet(input, rotation, 0.0f, 360.0f, "rotation values",
                             error_message);
}

bool has_obj_content(const std::filesystem::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return false;
  }

  bool has_vertex = false;
  bool has_face = false;
  std::string line;

  while (std::getline(file, line)) {
    const std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#') {
      continue;
    }

    if (trimmed.rfind("v ", 0) == 0) {
      has_vertex = true;
    }
    if (trimmed.rfind("f ", 0) == 0) {
      has_face = true;
    }

    if (has_vertex && has_face) {
      return true;
    }
  }

  return false;
}

bool validate_obj_path(const std::string& input, std::string& error_message) {
  const std::filesystem::path path(input);

  if (path.extension() != ".obj") {
    error_message = "Expected a .obj file path.";
    return false;
  }

  std::error_code error;
  if (!std::filesystem::exists(path, error) || error) {
    error_message = "File does not exist.";
    return false;
  }

  if (!std::filesystem::is_regular_file(path, error) || error) {
    error_message = "Path is not a regular file.";
    return false;
  }

  std::ifstream file(path);
  if (!file.good()) {
    error_message = "File is not readable.";
    return false;
  }

  if (!has_obj_content(path)) {
    error_message = "File does not look like a valid OBJ mesh.";
    return false;
  }

  return true;
}

bool read_obj_path(std::string& path) {
  while (true) {
    std::cout << "Enter path to a readable .obj file:\n> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      std::cin.clear();
      std::cout << "Input error. Try again or type \"exit\".\n";
      continue;
    }

    input = trim(input);
    if (is_exit_command(input)) {
      return false;
    }

    std::string error_message;
    if (validate_obj_path(input, error_message)) {
      path = input;
      return true;
    }

    std::cout << "Invalid file path: " << error_message << "\n";
  }
}

void handle_object_input(Renderer::World& world) {
  std::string path;

  if (!read_obj_path(path)) {
    std::cout << "Object input cancelled.\n";
    return;
  }

  try {
    auto mesh = Renderer::load_mesh_from_file(path);
    Renderer::TriangulatedObject object(std::move(mesh));
    world.add_object(std::move(object));
    std::cout << "Object added to scene.\n";
  } catch (const std::runtime_error& e) {
    std::cout << "Could not import object: " << e.what() << "\n";
  }
}

bool read_rgb_color(RgbColor& color) {
  while (true) {
    std::cout << "Enter RGB color (example: 255 128 0), or type \"exit\":\n> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      std::cin.clear();
      std::cout << "Input error. Try again or type \"exit\".\n";
      continue;
    }

    if (is_exit_command(input)) {
      return false;
    }

    std::string error_message;
    if (parse_rgb_color(input, color, error_message)) {
      return true;
    }

    std::cout << "Invalid RGB color: " << error_message << ".\n";
  }
}

void handle_color_input(Renderer::World& world) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    std::cout << "No selected object to recolor.\n";
    return;
  }

  RgbColor color{};
  if (!read_rgb_color(color)) {
    std::cout << "Color input cancelled.\n";
    return;
  }

  auto& object_color = selected_object->color();
  object_color[0] = static_cast<float>(color[0]) / 255.0f;
  object_color[1] = static_cast<float>(color[1]) / 255.0f;
  object_color[2] = static_cast<float>(color[2]) / 255.0f;

  std::cout << "Selected object color updated to RGB("
            << color[0] << ", " << color[1] << ", " << color[2] << ").\n";
}

bool read_object_position(VectorInput& position) {
  while (true) {
    std::cout << "Enter position coordinates (example: 0 1.5 -3), "
                 "or type \"exit\":\n> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      std::cin.clear();
      std::cout << "Input error. Try again or type \"exit\".\n";
      continue;
    }

    if (is_exit_command(input)) {
      return false;
    }

    std::string error_message;
    if (parse_object_position(input, position, error_message)) {
      return true;
    }

    std::cout << "Invalid position: " << error_message << ".\n";
  }
}

void handle_position_input(Renderer::World& world) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    std::cout << "No selected object to move.\n";
    return;
  }

  VectorInput position{};
  if (!read_object_position(position)) {
    std::cout << "Position input cancelled.\n";
    return;
  }

  selected_object->transform().position =
      Renderer::Vector3(position[0], position[1], position[2]);

  std::cout << "Selected object position updated to ("
            << position[0] << ", " << position[1] << ", " << position[2]
            << ").\n";
}

bool read_object_scale(VectorInput& scale) {
  while (true) {
    std::cout << "Enter scale by X Y Z (example: 1 1.5 0.5), "
                 "or type \"exit\":\n> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      std::cin.clear();
      std::cout << "Input error. Try again or type \"exit\".\n";
      continue;
    }

    if (is_exit_command(input)) {
      return false;
    }

    std::string error_message;
    if (parse_object_scale(input, scale, error_message)) {
      return true;
    }

    std::cout << "Invalid scale: " << error_message << ".\n";
  }
}

void handle_scale_input(Renderer::World& world) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    std::cout << "No selected object to scale.\n";
    return;
  }

  VectorInput scale{};
  if (!read_object_scale(scale)) {
    std::cout << "Scale input cancelled.\n";
    return;
  }

  selected_object->transform().scale =
      Renderer::Vector3(scale[0], scale[1], scale[2]);

  std::cout << "Selected object scale updated to ("
            << scale[0] << ", " << scale[1] << ", " << scale[2] << ").\n";
}

bool read_object_rotation(VectorInput& rotation) {
  while (true) {
    std::cout << "Enter rotation by X Y Z in degrees (example: 0 90 180), "
                 "or type \"exit\":\n> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      std::cin.clear();
      std::cout << "Input error. Try again or type \"exit\".\n";
      continue;
    }

    if (is_exit_command(input)) {
      return false;
    }

    std::string error_message;
    if (parse_object_rotation(input, rotation, error_message)) {
      return true;
    }

    std::cout << "Invalid rotation: " << error_message << ".\n";
  }
}

void handle_rotation_input(Renderer::World& world) {
  auto* selected_object = world.selected_object();
  if (selected_object == nullptr) {
    std::cout << "No selected object to rotate.\n";
    return;
  }

  VectorInput rotation{};
  if (!read_object_rotation(rotation)) {
    std::cout << "Rotation input cancelled.\n";
    return;
  }

  selected_object->transform().rotation =
      Renderer::Vector3(Renderer::to_radians(rotation[0]),
                        Renderer::to_radians(rotation[1]),
                        Renderer::to_radians(rotation[2]));

  std::cout << "Selected object rotation updated to ("
            << rotation[0] << ", " << rotation[1] << ", " << rotation[2]
            << ") degrees.\n";
}

void print_frame_time(float delta_time, float& accumulator) {
  accumulator += delta_time;
  if (accumulator < 1.0f) {
    return;
  }

  std::cout << "Frame time: " << delta_time * 1000.0f << " ms\n";
  accumulator = 0.0f;
}

KeyboardAction action_from_event(const sf::Event& event) {
  if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
    if (text->unicode == U'+') {
      return KeyboardAction::Import;
    }
  }

  const auto* key_pressed = event.getIf<sf::Event::KeyPressed>();
  if (key_pressed == nullptr) {
    return KeyboardAction::None;
  }

  if (key_pressed->code == sf::Keyboard::Key::Add) {
    return KeyboardAction::Import;
  }
  if (key_pressed->code == sf::Keyboard::Key::E) {
    return KeyboardAction::Edit;
  }
  if (key_pressed->code == sf::Keyboard::Key::B) {
    return KeyboardAction::Back;
  }
  if (key_pressed->code == sf::Keyboard::Key::Right) {
    return KeyboardAction::NextObject;
  }
  if (key_pressed->code == sf::Keyboard::Key::Left) {
    return KeyboardAction::PreviousObject;
  }
  if (key_pressed->code == sf::Keyboard::Key::C) {
    return KeyboardAction::Color;
  }
  if (key_pressed->code == sf::Keyboard::Key::P) {
    return KeyboardAction::Position;
  }
  if (key_pressed->code == sf::Keyboard::Key::S) {
    return KeyboardAction::Scale;
  }
  if (key_pressed->code == sf::Keyboard::Key::R) {
    return KeyboardAction::Rotation;
  }

  return KeyboardAction::None;
}

void render_frame(Renderer::Renderer& renderer, Renderer::Window& window,
                  const Renderer::TriangulatedObject* highlighted_object = nullptr) {
  renderer.draw_scene(highlighted_object);
  window.display();
}

}  // namespace

int main() {
  try {
    Renderer::Window window(800, 600, "test");
    Renderer::World world;
    Renderer::Camera camera;
    Renderer::Renderer renderer(window, world, camera);

    AppState state = AppState::Rendering;
    sf::Clock clock;
    float frame_time_accumulator = 0.0f;

    std::cout << "Rendering mode. Press + to add an OBJ object.\n";
    std::cout << "Press E to enter edit mode when objects exist.\n";
    std::cout << "Press C in edit mode to change selected object color.\n";
    std::cout << "Press P in edit mode to change selected object position.\n";
    std::cout << "Press S in edit mode to change selected object scale.\n";
    std::cout << "Press R in edit mode to change selected object rotation.\n";
    std::cout << "Press B to return from edit mode.\n";

    while (window.is_running()) {
      const float delta_time = clock.restart().asSeconds();
      KeyboardAction keyboard_action = KeyboardAction::None;
      const bool has_objects = !world.empty();

      while (const auto event = window.poll_event()) {
        window.handle_event(*event);
        const KeyboardAction next_action = action_from_event(*event);
        if (next_action != KeyboardAction::None) {
          keyboard_action = next_action;
        }
      }

      if (!window.is_running()) {
        continue;
      }

      switch (state) {
        case AppState::Rendering: {
          if (keyboard_action == KeyboardAction::Import) {
            switch_state(state, AppState::ObjectInput);
            break;
          }
          if (keyboard_action == KeyboardAction::Edit && has_objects) {
            switch_state(state, AppState::Edit);
            break;
          }
          if (keyboard_action == KeyboardAction::Edit && !has_objects) {
            std::cout << "Edit mode is available only when World has objects.\n";
          }

          update_camera(camera, window, delta_time);
          render_frame(renderer, window);
          print_frame_time(delta_time, frame_time_accumulator);
          break;
        }

        case AppState::ObjectInput: {
          handle_object_input(world);
          window.clear_input_state();
          switch_state(state, AppState::Rendering);
          clock.restart();
          frame_time_accumulator = 0.0f;
          break;
        }

        case AppState::ColorInput: {
          handle_color_input(world);
          window.clear_input_state();
          switch_state(state, AppState::Edit);
          clock.restart();
          frame_time_accumulator = 0.0f;
          break;
        }

        case AppState::PositionInput: {
          handle_position_input(world);
          window.clear_input_state();
          switch_state(state, AppState::Edit);
          clock.restart();
          frame_time_accumulator = 0.0f;
          break;
        }

        case AppState::ScaleInput: {
          handle_scale_input(world);
          window.clear_input_state();
          switch_state(state, AppState::Edit);
          clock.restart();
          frame_time_accumulator = 0.0f;
          break;
        }

        case AppState::RotationInput: {
          handle_rotation_input(world);
          window.clear_input_state();
          switch_state(state, AppState::Edit);
          clock.restart();
          frame_time_accumulator = 0.0f;
          break;
        }

        case AppState::Edit: {
          if (keyboard_action == KeyboardAction::Back) {
            switch_state(state, AppState::Rendering);
            break;
          }
          if (!has_objects) {
            switch_state(state, AppState::Rendering);
            break;
          }
          if (keyboard_action == KeyboardAction::Color) {
            switch_state(state, AppState::ColorInput);
            break;
          }
          if (keyboard_action == KeyboardAction::Position) {
            switch_state(state, AppState::PositionInput);
            break;
          }
          if (keyboard_action == KeyboardAction::Scale) {
            switch_state(state, AppState::ScaleInput);
            break;
          }
          if (keyboard_action == KeyboardAction::Rotation) {
            switch_state(state, AppState::RotationInput);
            break;
          }
          if (keyboard_action == KeyboardAction::NextObject) {
            world.select_next_object();
          }
          if (keyboard_action == KeyboardAction::PreviousObject) {
            world.select_previous_object();
          }

          render_frame(renderer, window, world.selected_object());
          print_frame_time(delta_time, frame_time_accumulator);
          break;
        }
      }
    }
  } catch (const std::runtime_error& e) {
    std::cout << e.what() << "\n";
    return 1;
  }

  return 0;
}
