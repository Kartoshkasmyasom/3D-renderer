#include "ImportMesh.hpp"
#include "Camera.hpp"
#include "TriangulatedObject.hpp"
#include "Shader.hpp"
#include "World.hpp"

#include <Eigen/Dense>

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
  "uniform vec4 objectColor;\n"
  "void main(){\n"
  "FragColor = objectColor;}";

int main() {

  sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

   sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "test",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );
    
    (void)window.setActive(true);

  GLenum err = glewInit();

  if(err != GLEW_OK) {
      std::cout << "glewInit failed: "  << glewGetErrorString(err) << "\n";
      return 1;
  }

  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";

  Renderer::World world;
  try {
    auto mesh = Renderer::load_mesh_from_file(
          "C:\\Users\\nasty\\3D-renderer\\file-import\\penis.obj");
      Renderer::TriangulatedObject object1(std::move(mesh));
      mesh = Renderer::load_mesh_from_file(
          "C:\\Users\\nasty\\3D-renderer\\file-import\\cube.obj");
      std::cout << "Successful import!\n";
    Renderer::TriangulatedObject object2(std::move(mesh));
    object1.color() = Eigen::Vector4f(1.0f, 0.55f, 0.57f, 1.0f);
    object2.color() = Eigen::Vector4f(0.35f, 0.75f, 1.0f, 1.0f);
    object2.transform().position = Eigen::Vector3f(0.5f, 0.8f, 0.8f);
    object2.transform().scale = Eigen::Vector3f(0.5f, 0.5f, 0.5f);
    object2.transform().rotation = Eigen::Vector3f(0.0f, 45.0f, 0.0f);
    world.add_object(std::move(object1));
    world.add_object(std::move(object2));
    }
  catch (const std::runtime_error& e) {
    std::cout << e.what() << "\n";
  }
  Renderer::Shader shader(vertexShaderSource, fragmentShaderSource);

  Renderer::Camera camera;

  bool running = true;
    while (running) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        const sf::Vector2u window_size = window.getSize();
        shader.set_mat4("view", camera.view_matrix());
        shader.set_mat4("projection", camera.projection_matrix(
            static_cast<float>(window_size.x),
            static_cast<float>(window_size.y)));

        for(auto it = world.begin(); it != world.end(); ++it) {
          shader.set_mat4("model", it->transform().matrix());
          shader.set_vec4("objectColor", it->color());
          it->draw();
        }
        window.display(); 

    }

  return 0;
}
