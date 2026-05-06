#include "Shader.hpp"

#include <iostream>


namespace Renderer {
Shader::Shader(const std::string& vertex_code, const std::string& fragment_code) {
    const char* c_vertex_code = vertex_code.c_str();
    const char* c_fragment_code = fragment_code.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &c_vertex_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &c_fragment_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    check_compile_errors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}


void Shader::check_compile_errors(unsigned int shader, std::string type) {
    int success;
    char info[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info);
            std::cout << "Error in shader compilation: " << type << "\n" << info << "\n\n\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info);
            std::cout << "Error in shader compilation: " << type << "\n" << info << "\n\n\n";
        }
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_vec3(const std::string& name, const Vector3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
}

void Shader::set_vec4(const std::string& name, const Vector4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
}

void Shader::set_mat4(const std::string& name,
                      const Matrix4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       value.data());
}
} // namespace Renderer
