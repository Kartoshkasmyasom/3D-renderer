#include "Window.hpp"

#include <GL/glew.h>

#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

namespace Renderer {
namespace {

void init_glew() {
  glewExperimental = GL_TRUE;
  const GLenum error = glewInit();
  if (error != GLEW_OK) {
    throw std::runtime_error(
        std::string("glewInit failed: ") +
        reinterpret_cast<const char*>(glewGetErrorString(error)));
  }

  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
}

}  // namespace

Window::Window(unsigned int width, unsigned int height,
               const std::string& title)
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

bool Window::is_running() const { return window_.isOpen(); }

std::optional<sf::Event> Window::poll_event() { return window_.pollEvent(); }

sf::RenderWindow& Window::render_window() { return window_; }

void Window::display() { window_.display(); }

sf::Vector2u Window::size() const { return window_.getSize(); }

}  // namespace Renderer
