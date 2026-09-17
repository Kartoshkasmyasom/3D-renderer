#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include <string>

namespace Renderer {

class Window {
 public:
  Window(unsigned int width, unsigned int height, const std::string& title);

  bool is_running() const;

  // pop the element on top of event queue and return it, or std::nullopt if the
  // queue is empty
  std::optional<sf::Event> poll_event();
  sf::RenderWindow& render_window();
  void display();
  sf::Vector2u size() const;

 private:
  sf::RenderWindow window_;
};

}  // namespace Renderer
