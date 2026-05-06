#include "Renderer.hpp"

#include <GL/glew.h>

#include <algorithm>
#include <string>

namespace Renderer {

namespace {

const std::string kVertexShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aNormal;\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 view;\n"
                                        "uniform mat4 projection;\n"
                                        "out vec3 Normal;\n"
                                        "void main(){\n"
                                        "Normal = mat3(transpose(inverse(model))) * "
                                        "aNormal;\n"
                                        "gl_Position = projection * view * model * "
                                        "vec4(aPos, 1.0);}\0";

const std::string kFragmentShaderSource = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "in vec3 Normal;\n"
                                          "uniform vec4 objectColor;\n"
                                          "uniform vec3 lightDirection;\n"
                                          "uniform vec3 lightColor;\n"
                                          "uniform vec3 ambientColor;\n"
                                          "void main(){\n"
                                          "vec3 normal = normalize(Normal);\n"
                                          "vec3 lightDir = normalize(-lightDirection);\n"
                                          "float diffuse = max(dot(normal, lightDir), "
                                          "0.0);\n"
                                          "vec3 lighting = ambientColor + diffuse * "
                                          "lightColor;\n"
                                          "FragColor = vec4(lighting * objectColor.rgb, "
                                          "objectColor.a);}";

}  // namespace

Renderer::Renderer(Window& window, World& world, Camera& camera)
    : window_(window),
      world_(world),
      camera_(camera),
      shader_(kVertexShaderSource, kFragmentShaderSource) {
  glEnable(GL_DEPTH_TEST);
}

namespace {

Vector4 highlighted_color(Vector4 color) {
  constexpr float brightness = 1.45f;
  constexpr float lift = 0.12f;

  for (int i = 0; i < 3; ++i) {
    color[i] = std::min(1.0f, color[i] * brightness + lift);
  }

  return color;
}

}  // namespace

void Renderer::draw_scene(const TriangulatedObject* highlighted_object) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_.use();
  const sf::Vector2u window_size = window_.size();
  shader_.set_mat4("view", camera_.view_matrix());
  shader_.set_mat4("projection",
                   camera_.projection_matrix(static_cast<float>(window_size.x),
                                             static_cast<float>(window_size.y)));
  shader_.set_vec3("lightDirection", light_.direction());
  shader_.set_vec3("lightColor", light_.color());
  shader_.set_vec3("ambientColor", ambient_light_.color());

  for (auto it = world_.begin(); it != world_.end(); ++it) {
    shader_.set_mat4("model", it->transform().matrix());
    const Vector4 color = &(*it) == highlighted_object
                              ? highlighted_color(it->color())
                              : it->color();
    shader_.set_vec4("objectColor", color);
    it->draw();
  }

  glBindVertexArray(0);
}
}  // namespace Renderer
