#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>

namespace Renderer {

GLenum glCheckError_(const char* file, int line);

#ifdef DEBUG
#define glCheckError() Renderer::glCheckError_(__FILE__, __LINE__)
#else
#define glCheckError() ((void)0)
#endif

#ifdef DEBUG
#define GL_CHECK(x)                                                      \
  do {                                                                   \
    x;                                                                   \
    GLenum error = glGetError();                                         \
    if (error != GL_NO_ERROR) {                                          \
      std::cerr << "OpenGL error " << error << " at " << __FILE__ << ":" \
                << __LINE__ << std::endl;                                \
      __debugbreak();                                                    \
    }                                                                    \
  } while (0)
#else
#define GL_CHECK(x) x
#endif

}  // namespace Renderer
