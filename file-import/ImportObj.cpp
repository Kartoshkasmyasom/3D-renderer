#include "ImportMesh.hpp"
#include "TriangulatedObject.hpp"

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

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main(){\n"
  "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main(){\n"
  "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

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
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success = 0;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout<<"Shader compilation failed: " << infoLog << std::endl;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  success = 0;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout<<"Fragment compilation failed: " << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  success = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout<<"Shader program link failed: " << infoLog << std::endl;
  }  

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
  GLuint indices[] = {0, 1, 3, 1, 2, 3};
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindVertexArray(0);
  glUseProgram(shaderProgram);
  // finish

  try {
    auto mesh = Renderer::load_mesh_from_file(
        "/home/nasty/Workspace/VSCodeprojects/3d-renderer/file-import/"
        "penis.obj");
    Renderer::TriangulatedObject object(std::move(mesh));
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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.display(); 
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

  return 0;
}