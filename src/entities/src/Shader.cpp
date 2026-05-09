#include "Shader.hpp"

#include <iostream>

namespace Renderer {

Shader::Shader(const std::string& vertex_code,
               const std::string& fragment_code) {
  const char* vertex_source = vertex_code.c_str();
  const char* fragment_source = fragment_code.c_str();

  const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, nullptr);
  glCompileShader(vertex);
  check_compile_errors(vertex, "VERTEX");

  const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, nullptr);
  glCompileShader(fragment);
  check_compile_errors(fragment, "FRAGMENT");

  id_ = glCreateProgram();
  glAttachShader(id_, vertex);
  glAttachShader(id_, fragment);
  glLinkProgram(id_);
  check_compile_errors(id_, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  glDeleteProgram(id_);
}

void Shader::check_compile_errors(GLuint shader, const std::string& type) {
  GLint success = 0;
  char info[1024] = {};

  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
      glGetShaderInfoLog(shader, 1024, nullptr, info);
      std::cout << "Error in shader compilation: " << type << "\n"
                << info << "\n";
    }
    return;
  }

  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(shader, 1024, nullptr, info);
    std::cout << "Error in shader linking: " << type << "\n" << info << "\n";
  }
}

void Shader::use() {
  glUseProgram(id_);
}

void Shader::set_bool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_int(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_vec3(const std::string& name, const Vector3& value) const {
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value.data());
}

void Shader::set_vec4(const std::string& name, const Vector4& value) const {
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value.data());
}

void Shader::set_mat4(const std::string& name, const Matrix4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     value.data());
}

}  // namespace Renderer
