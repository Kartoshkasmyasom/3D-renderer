cmake_minimum_required(VERSION 3.10)

project(entities)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(${PROJECT_NAME} 
    include/MeshData.hpp
    include/TriangulatedObject.hpp 
    include/Shader.hpp 
    src/TriangulatedObject.cpp
    src/Shader.cpp
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
    project-exceptions
)
