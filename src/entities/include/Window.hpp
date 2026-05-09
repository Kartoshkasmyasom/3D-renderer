#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <optional>
#include <set>
#include <string>

namespace Renderer {

class Window {
 public:
  Window(unsigned int width, unsigned int height, const std::string& title);

  bool is_running() const;
  bool is_key_pressed(sf::Keyboard::Key key) const;
  void clear_input_state();
  std::optional<sf::Event> poll_event();
  void handle_event(const sf::Event& event);
  sf::RenderWindow& render_window();
  void display();
  sf::Vector2u size() const;

 private:
  sf::RenderWindow window_;
  std::set<sf::Keyboard::Key> pressed_keys_;
  bool running_ = true;
};

}  // namespace Renderer
