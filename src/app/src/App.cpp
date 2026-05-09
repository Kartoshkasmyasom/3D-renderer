#include "App.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <stdexcept>
#include <string>

namespace Renderer {

App::App()
    : window_(800, 600, "test"),
      world_(),
      camera_(),
      renderer_(world_, camera_),
      importer_(),
      dialogue_(),
      model_context_{world_, camera_, renderer_, importer_},
      ui_context_{window_, dialogue_} {
  const auto window_size = window_.size();
  dialogue_.print_message("Window resolution: " +
                          std::to_string(window_size.x) + "x" +
                          std::to_string(window_size.y));
  dialogue_.print_message(help_rendering);
}

void App::switch_state(AppState next_state) {
  state_ = next_state;

  if (state_ == AppState::Rendering) {
    dialogue_.print_message(help_rendering);
    return;
  }

  if (state_ == AppState::Edit) {
    dialogue_.print_message(help_edit);
  }
}

void App::update_camera(float delta_time) {
  constexpr float movement_speed = 3.0f;
  constexpr float rotation_speed = 90.0f;

  const float movement = movement_speed * delta_time;
  const float rotation = rotation_speed * delta_time;

  if (window_.is_key_pressed(sf::Keyboard::Key::W)) {
    camera_.move_forward(movement);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::S)) {
    camera_.move_forward(-movement);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::D)) {
    camera_.move_right(movement);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::A)) {
    camera_.move_right(-movement);
  }

  if (window_.is_key_pressed(sf::Keyboard::Key::Left)) {
    camera_.rotate_yaw(-rotation);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::Right)) {
    camera_.rotate_yaw(rotation);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::Up)) {
    camera_.rotate_pitch(rotation);
  }
  if (window_.is_key_pressed(sf::Keyboard::Key::Down)) {
    camera_.rotate_pitch(-rotation);
  }
}

void App::render_frame(const TriangulatedObject* highlighted_object) {
  renderer_.draw_scene(window_.size(), highlighted_object);
  window_.display();
}

void App::print_frame_time(float delta_time, float& accumulator) const {
  accumulator += delta_time;
  if (accumulator < 1.0f) {
    return;
  }

  std::cout << "Frame time: " << delta_time * 1000.0f << " ms\n";
  accumulator = 0.0f;
}

int App::run() {
  try {
    sf::Clock clock;
    float frame_time_accumulator = 0.0f;

    while (window_.is_running()) {
      const float delta_time = clock.restart().asSeconds();
      KeyboardAction keyboard_action = KeyboardAction::None;

      while (auto event = window_.poll_event()) {
        window_.handle_event(*event);
        const KeyboardAction next_action = action_from_event(*event);
        if (next_action != KeyboardAction::None) {
          keyboard_action = next_action;
        }
      }

      if (!window_.is_running()) {
        continue;
      }

      switch (state_) {
        case AppState::Rendering: {
          if (keyboard_action == KeyboardAction::Edit) {
            switch_state(AppState::Edit);
            break;
          }

          update_camera(delta_time);
          render_frame();
          print_frame_time(delta_time, frame_time_accumulator);
          break;
        }

        case AppState::Edit: {
          if (keyboard_action == KeyboardAction::Back) {
            switch_state(AppState::Rendering);
            break;
          }
          if (keyboard_action == KeyboardAction::NextObject &&
              !world_.empty()) {
            world_.select_next_object();
            break;
          }
          if (keyboard_action == KeyboardAction::PreviousObject &&
              !world_.empty()) {
            world_.select_previous_object();
            break;
          }
          if (keyboard_action == KeyboardAction::Import) {
            handle_console_command<ConsoleAction::ImportObject>();
            break;
          }
          if (keyboard_action == KeyboardAction::Color) {
            handle_console_command<ConsoleAction::ChangeColor>();
            break;
          }
          if (keyboard_action == KeyboardAction::Position) {
            handle_console_command<ConsoleAction::MoveObject>();
            break;
          }
          if (keyboard_action == KeyboardAction::Scale) {
            handle_console_command<ConsoleAction::ScaleObject>();
            break;
          }
          if (keyboard_action == KeyboardAction::Rotation) {
            handle_console_command<ConsoleAction::RotateObject>();
            break;
          }
          if (keyboard_action == KeyboardAction::Delete) {
            handle_console_command<ConsoleAction::DeleteObject>();
            break;
          }

          render_frame(world_.selected_object());
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

KeyboardAction App::action_from_event(const sf::Event& event) const {
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
  if (key_pressed->code == sf::Keyboard::Key::Delete) {
    return KeyboardAction::Delete;
  }

  return KeyboardAction::None;
}

template <ConsoleAction Action>
void App::handle_console_command() {
  if constexpr (Action != ConsoleAction::ImportObject) {
    if (world_.empty()) {
      dialogue_.print_message("No selected object.");
      return;
    }
  }

  using Messages = typename ConsoleActionTraits<Action>::Messages;
  using Validator = typename ConsoleActionTraits<Action>::Validator;
  using ConsoleCommand = typename ConsoleActionTraits<Action>::ConsoleCommand;

  auto request = dialogue_.get_user_request(Messages{}, Validator{});
  if (!request.has_value()) {
    return;
  }

  ConsoleCommand command;
  try {
    dialogue_.print_message("Applying changes...");
    command.execute(model_context_, *request);
    dialogue_.print_message("Success!");
  } catch (const std::runtime_error& e) {
    throw std::runtime_error(std::string("Failed to execute command: ") +
                             e.what());
  }
}

}  // namespace Renderer
