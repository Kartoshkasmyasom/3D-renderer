#include "ImportMesh.hpp"
#include "Object.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

// GLenum glCheckError_(const char *file, int line) {
//   GLenum errorCode;
//   while ((errorCode = glGetError()) != GL_NO_ERROR){
//     std::string error;
//     switch (errorCode) {
//       case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
//       case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
//       case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
//       case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
//       case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
//       case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
//       case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
//       }
//       std::cout << error << " | " << file << " (" << line << ")" << std::endl;
//   }
//   return errorCode;
// }
// #define glCheckError() glCheckError_(__FILE__, __LINE__)

// #ifdef DEBUG
// #define GL_CHECK(x) do { \
//     x; \
//     GLenum error = glGetError(); \
//     if (error != GL_NO_ERROR) { \
//         std::cerr << "OpenGL error " << error << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
//         __builtin_trap(); \
//     } \
// } while(0)
// #else
// #define GL_CHECK(x) x
// #endif

int main() {
  sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

   sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "test",
        sf::Style::Default,
        settings
    );
    
    window.setActive(true);

  GLenum err = glewInit();

  if(err != GLEW_OK) {
      std::cout << "glewInit failed: "  << glewGetErrorString(err) << "\n";
      return 1;
  }

  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";

  try {
    auto mesh = Renderer::load_mesh_from_file(
        "/home/nasty/Workspace/VSCodeprojects/3d-renderer/file-import/"
        "penis.obj");
    Renderer::Object object(std::move(mesh));
    std::cout << "Successful import!\n";
  } catch (std::runtime_error& e) {
    std::cout << e.what();
  }

  bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
        }
        
        window.display(); 
    }
  return 0;
}