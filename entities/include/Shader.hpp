#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <string>

namespace Renderer {

using Matrix4 = Eigen::Matrix4f;
using Vector3 = Eigen::Vector3f;
using Vector4 = Eigen::Vector4f;

class Shader {
public:
    GLuint ID;

    Shader(const std::string& vertex_code, const std::string& fragment_code);
    ~Shader();

    void use();
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, const Vector3& value) const;
    void set_vec4(const std::string& name, const Vector4& value) const;
    void set_mat4(const std::string& name, const Matrix4& value) const;
private:
     void check_compile_errors(GLuint shader, std::string type);
};
} // namespace Renderer
