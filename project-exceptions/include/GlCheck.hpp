#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

namespace Renderer{

GLenum glCheckError_(const char *file, int line);

#ifdef DEBUG
#define glCheckError() glCheckError_(__FILE__, __LINE__)
#else
#define glCheckError() ((void)0)
#endif

#ifdef DEBUG
#define GL_CHECK(x) do { \
    x; \
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
        std::cerr << "OpenGL error " << error << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        __builtin_trap(); \
    } \
} while(0)
#else
#define GL_CHECK(x) x
#endif
} // Renderer