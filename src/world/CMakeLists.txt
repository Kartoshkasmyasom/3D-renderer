cmake_minimum_required(VERSION 3.10)

project(world)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
    include/object/MeshData.hpp
    include/object/TriangulatedObject.hpp
    include/object/Transform.hpp
    include/World.hpp
    include/Camera.hpp
    include/Lighting.hpp
    include/commands/Commands.hpp
    include/commands/CommandTraits.hpp
    include/Requests.hpp
    src/object/TriangulatedObject.cpp
    src/object/Transform.cpp
    src/World.cpp
    src/Camera.cpp
    src/Lighting.cpp
    src/commands/Commands.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/app/include
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
