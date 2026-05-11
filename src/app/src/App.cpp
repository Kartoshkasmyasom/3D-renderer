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
  const auto window_size = ui_context_.window.size();
  ui_context_.dialogue.print_message("Window resolution: " +
                                     std::to_string(window_size.x) + "x" +
                                     std::to_string(window_size.y));
  ui_context_.dialogue.print_message(help_rendering);
}

void App::switch_state(AppState next_state) {
  state_ = next_state;

  if (state_ == AppState::Rendering) {
    ui_context_.dialogue.print_message(help_rendering);
    return;
  }

  if (state_ == AppState::Edit) {
    ui_context_.dialogue.print_message(help_edit);
  }
}

void App::update_camera(float delta_time) {
  constexpr float movement_speed = 3.0f;
  constexpr float rotation_speed = 90.0f;

  const float movement = movement_speed * delta_time;
  const float rotation = rotation_speed * delta_time;

  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::W)) {
    model_context_.camera.move_forward(movement);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::S)) {
    model_context_.camera.move_forward(-movement);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::D)) {
    model_context_.camera.move_right(movement);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::A)) {
    model_context_.camera.move_right(-movement);
  }

  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::Left)) {
    model_context_.camera.rotate_yaw(-rotation);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::Right)) {
    model_context_.camera.rotate_yaw(rotation);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::Up)) {
    model_context_.camera.rotate_pitch(rotation);
  }
  if (ui_context_.window.is_key_pressed(sf::Keyboard::Key::Down)) {
    model_context_.camera.rotate_pitch(-rotation);
  }
}

void App::render_frame(const TriangulatedObject* highlighted_object) {
  model_context_.renderer.draw_scene(ui_context_.window.size(),
                                     highlighted_object);
  ui_context_.window.display();
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

    while (ui_context_.window.is_running()) {
      const float delta_time = clock.restart().asSeconds();
      KeyboardAction keyboard_action = KeyboardAction::None;

      while (auto event = ui_context_.window.poll_event()) {
        ui_context_.window.handle_event(*event);
        const KeyboardAction next_action = action_from_event(*event);
        if (next_action != KeyboardAction::None) {
          keyboard_action = next_action;
        }
      }

      if (!ui_context_.window.is_running()) {
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
              !model_context_.world.empty()) {
            model_context_.world.select_next_object();
            break;
          }
          if (keyboard_action == KeyboardAction::PreviousObject &&
              !model_context_.world.empty()) {
            model_context_.world.select_previous_object();
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

          render_frame(model_context_.world.selected_object());
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
    if (model_context_.world.empty()) {
      ui_context_.dialogue.print_message("No selected object.");
      return;
    }
  }

  using Messages = typename ConsoleActionTraits<Action>::Messages;
  using Validator = typename ConsoleActionTraits<Action>::Validator;
  using ConsoleCommand = typename ConsoleActionTraits<Action>::ConsoleCommand;

  auto request =
      ui_context_.dialogue.get_user_request(Messages{}, Validator{});
  if (!request.has_value()) {
    return;
  }

  ConsoleCommand command;
  try {
    ui_context_.dialogue.print_message("Applying changes...");
    command.execute(model_context_, *request);
    ui_context_.dialogue.print_message("Success!");
  } catch (const std::runtime_error& e) {
    throw std::runtime_error(std::string("Failed to execute command: ") +
                             e.what());
  }
}

}  // namespace Renderer
