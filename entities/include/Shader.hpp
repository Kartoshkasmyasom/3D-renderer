#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace Renderer {
class Shader {
public:
    GLuint ID;

    Shader(const std::string& vertex_code, const std::string& fragment_code);
    ~Shader();

    void use();
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
private:
     void check_compile_errors(GLuint shader, std::string type);
};
} // namespace Renderer