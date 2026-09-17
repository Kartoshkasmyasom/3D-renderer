#include "App.hpp"

#include <GL/glew.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "MessageTraits.hpp"
#include "Validators.hpp"

namespace Renderer {

App::App()
    : window_(800, 600, "test"),
      importer_(),
      world_(),
      renderer_(world_),
      dialogue_() {
  const auto window_size = window_.size();
  dialogue_.print_message(
      "Window resolution: " + std::to_string(window_size.x) + "x" +
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

void App::render_frame(const TriangulatedObject* highlighted_object) {
  renderer_.draw_scene(window_.size(), highlighted_object);
  window_.display();
}

int App::run() {
  try {
    sf::Clock clock;

    while (window_.is_running()) {
      delta_time_ = clock.restart().asSeconds();
      Action action = Action::None;
      while (auto event = window_.poll_event()) {
        if (event->is<sf::Event::Closed>()) {
          window_.render_window().close();
        }
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
          glViewport(0, 0, resized->size.x, resized->size.y);
        }
        if (const auto* released = event->getIf<sf::Event::KeyReleased>()) {
          pressed_keys_.erase(released->code);
        }
        if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
          if (text->unicode == U'+') {
            action = Action::ImportObject;
          }
        }
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
          pressed_keys_.insert(key->code);
          switch (key->code) {
            case sf::Keyboard::Key::Add:
              action = Action::ImportObject;
              break;
            case sf::Keyboard::Key::E:
              action = Action::Edit;
              break;
            case sf::Keyboard::Key::B:
              action = Action::Back;
              break;
            case sf::Keyboard::Key::Right:
              if (state_ == AppState::Edit) {
                action = Action::NextObject;
              }
              break;
            case sf::Keyboard::Key::Left:
              if (state_ == AppState::Edit) {
                action = Action::PreviousObject;
              }
              break;
            case sf::Keyboard::Key::C:
              action = Action::ChangeColor;
              break;
            case sf::Keyboard::Key::P:
              action = Action::MoveObject;
              break;
            case sf::Keyboard::Key::S:
              action = Action::ScaleObject;
              break;
            case sf::Keyboard::Key::R:
              action = Action::RotateObject;
              break;
            case sf::Keyboard::Key::Delete:
              action = Action::DeleteObject;
              break;
            default:
              break;
          }
        }
      }

      if (!window_.is_running()) {
        continue;
      }
      switch (state_) {
        case AppState::Rendering: {
          if (action == Action::Edit) {
            switch_state(AppState::Edit);
            break;
          }

          if (!window_.render_window().hasFocus()) {
            pressed_keys_.clear();
            sf::sleep(sf::milliseconds(50));
            // После возвращения фокуса delta_time_ не должен учитывать паузу.
            clock.restart();
            continue;
          }

          if (pressed_keys_.count(sf::Keyboard::Key::W)) {
            handle_keyboard_command<Action::MoveForward>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::S)) {
            handle_keyboard_command<Action::MoveBackward>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::D)) {
            handle_keyboard_command<Action::MoveRight>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::A)) {
            handle_keyboard_command<Action::MoveLeft>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::Left)) {
            handle_keyboard_command<Action::RotateLeft>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::Right)) {
            handle_keyboard_command<Action::RotateRight>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::Up)) {
            handle_keyboard_command<Action::RotateUp>();
          }
          if (pressed_keys_.count(sf::Keyboard::Key::Down)) {
            handle_keyboard_command<Action::RotateDown>();
          }
          render_frame();
          break;
        }

        case AppState::Edit: {
          if (action == Action::Back) {
            switch_state(AppState::Rendering);
            break;
          }
          if (action == Action::NextObject) {
            handle_keyboard_command<Action::NextObject>();
            break;
          }
          if (action == Action::PreviousObject) {
            handle_keyboard_command<Action::PreviousObject>();
            break;
          }
          if (action == Action::ImportObject) {
            handle_console_command<Action::ImportObject>();
            break;
          }
          if (action == Action::ChangeColor) {
            handle_console_command<Action::ChangeColor>();
            break;
          }
          if (action == Action::MoveObject) {
            handle_console_command<Action::MoveObject>();
            break;
          }
          if (action == Action::ScaleObject) {
            handle_console_command<Action::ScaleObject>();
            break;
          }
          if (action == Action::RotateObject) {
            handle_console_command<Action::RotateObject>();
            break;
          }
          if (action == Action::DeleteObject) {
            handle_console_command<Action::DeleteObject>();
            break;
          }

          if (!window_.render_window().hasFocus()) {
            pressed_keys_.clear();
            sf::sleep(sf::milliseconds(50));
            // После возвращения фокуса delta_time_ не должен учитывать паузу.
            clock.restart();
            continue;
          }

          render_frame(world_.selected_object());
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

template <Action action>
void App::handle_console_command() {
  if constexpr (action != Action::ImportObject) {
    if (world_.empty()) {
      dialogue_.print_message("No selected object.");
      return;
    }
  }

  using Messages = MessageTraits<action>;
  using ActionValidator = Validator<action>;
  using ActionCommand = typename ActionTraits<action>::Command;

  while (true) {
    auto request = dialogue_.get_user_request(Messages{}, ActionValidator{});
    // Discard keys accumulated during the blocking console dialogue.
    while (auto event = window_.poll_event()) {
      if (event->is<sf::Event::Closed>()) {
        window_.render_window().close();
      }
      if (const auto* resized = event->getIf<sf::Event::Resized>()) {
        glViewport(0, 0, resized->size.x, resized->size.y);
      }
    }
    pressed_keys_.clear();
    if (!request.has_value()) {
      return;
    }

    ActionCommand command;
    try {
      dialogue_.print_message("Applying changes...");
      if constexpr (action == Action::ImportObject) {
        auto mesh = importer_.load_mesh_from_file(request->path);
        TriangulatedObject object(std::move(mesh));
        command.execute(world_, std::move(object));
      } else {
        command.execute(world_, *request);
      }
      dialogue_.print_message("Success!");
      return;
    } catch (const std::runtime_error& e) {
      dialogue_.print_message(std::string("Failed to execute command: ") +
                              e.what());
      dialogue_.print_message("Try again or type \"exit\" to cancel.");
    }
  }
}

template <Action action>
void App::handle_keyboard_command() {
  using Request = typename ActionTraits<action>::Request;
  using ActionCommand = typename ActionTraits<action>::Command;

  Request request{};
  if constexpr (!std::is_same_v<Request, SelectObjectRequest>) {
    request.delta_time = delta_time_;
  }

  ActionCommand command;
  command.execute(world_, request);
}

}  // namespace Renderer
