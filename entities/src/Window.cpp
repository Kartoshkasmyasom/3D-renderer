#include <GL/glew.h>

#include "Window.hpp"

#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <iostream>
#include <stdexcept>
#include <string>

namespace Renderer {

namespace {

void init_glew() {
  glewExperimental = GL_TRUE;
  const GLenum err = glewInit();
  if (err != GLEW_OK) {
    throw std::runtime_error(
        std::string("glewInit failed: ") +
        reinterpret_cast<const char*>(glewGetErrorString(err)));
  }

  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
}

}  // namespace

Window::Window(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode({width, height}), title, sf::Style::Default,
              sf::State::Windowed, [] {
                sf::ContextSettings settings;
                settings.majorVersion = 3;
                settings.minorVersion = 3;
                settings.depthBits = 24;
                return settings;
              }()) {
  (void)window_.setActive(true);
  window_.setKeyRepeatEnabled(false);
  init_glew();
  glViewport(0, 0, width, height);
}

bool Window::is_running() const {
  return running_ && window_.isOpen();
}

bool Window::is_key_pressed(sf::Keyboard::Key key) const {
  return pressed_keys_.find(key) != pressed_keys_.end();
}

void Window::clear_input_state() {
  pressed_keys_.clear();
}

std::optional<sf::Event> Window::poll_event() {
  return window_.pollEvent();
}

void Window::handle_event(const sf::Event& event) {
  if (event.is<sf::Event::Closed>()) {
    running_ = false;
    window_.close();
  }

  if (const auto* resized = event.getIf<sf::Event::Resized>()) {
    glViewport(0, 0, resized->size.x, resized->size.y);
  }

  if (const auto* key_pressed = event.getIf<sf::Event::KeyPressed>()) {
    pressed_keys_.insert(key_pressed->code);
  }

  if (const auto* key_released = event.getIf<sf::Event::KeyReleased>()) {
    pressed_keys_.erase(key_released->code);
  }
}

sf::RenderWindow& Window::render_window() {
  return window_;
}

void Window::display() {
  window_.display();
}

sf::Vector2u Window::size() const {
  return window_.getSize();
}

}  // namespace Renderer
