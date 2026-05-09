#include "App.hpp"

#include <exception>
#include <iostream>

int main() {
  try {
    Renderer::App app;
    app.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
