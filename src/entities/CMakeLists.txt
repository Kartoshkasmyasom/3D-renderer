cmake_minimum_required(VERSION 3.10)

project(entities)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
    include/MeshData.hpp
    include/TriangulatedObject.hpp
    include/Shader.hpp
    include/Transform.hpp
    include/World.hpp
    include/Camera.hpp
    include/light/AmbientLight.hpp
    include/light/DirectionalLight.hpp
    include/Window.hpp
    include/Renderer.hpp
    src/TriangulatedObject.cpp
    src/Shader.cpp
    src/Transform.cpp
    src/World.cpp
    src/Camera.cpp
    src/light/AmbientLight.cpp
    src/light/DirectionalLight.cpp
    src/Window.cpp
    src/Renderer.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/project-exceptions/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    OpenGL::GL
    GLEW::GLEW
    Eigen3::Eigen
    SFML::Graphics
    SFML::Window
    SFML::System
    project-exceptions
)
