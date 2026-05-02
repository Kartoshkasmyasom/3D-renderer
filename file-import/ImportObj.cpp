#include "ImportMesh.hpp"
#include "TriangulatedObject.hpp"
#include "Shader.hpp"
#include "World.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

const std::string vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main(){\n"
  "gl_Position = projection * view * model * vec4(aPos, 1.0);}\0";

const std::string fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main(){\n"
  "FragColor = vec4(1.0f, 0.55f, 0.57f, 1.0f);}";

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

   // start
  // finish

  // try {
  //   auto mesh = Renderer::load_mesh_from_file(
  //       "/home/nasty/Workspace/VSCodeprojects/3d-renderer/3D-renderer/file-import/"
  //       "penis.obj");
  //   Renderer::TriangulatedObject object(std::move(mesh));
  //   std::cout << "Successful import!\n";
  // } catch (std::runtime_error& e) {
  //   std::cout << e.what();
  //}

  Renderer::World world;
  auto mesh = Renderer::load_mesh_from_file(
        "/home/nasty/Workspace/VSCodeprojects/3d-renderer/3D-renderer/file-import/"
        "penis.obj");
    Renderer::TriangulatedObject object1(std::move(mesh));
    mesh = Renderer::load_mesh_from_file(
        "/home/nasty/Workspace/VSCodeprojects/3d-renderer/3D-renderer/file-import/"
        "cube.obj");
    std::cout << "Successful import!\n";
   Renderer::TriangulatedObject object2(std::move(mesh));
  world.add_object(std::move(object1));
  world.add_object(std::move(object2));
  Renderer::Shader shader(vertexShaderSource, fragmentShaderSource);

  bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        for(auto it = world.begin(); it != world.end(); ++it) {
          it->draw();
        }
        window.display(); 

    }

  return 0;
}