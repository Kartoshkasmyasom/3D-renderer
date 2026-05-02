cmake_minimum_required(VERSION 3.10)

project(3d-renderer)

#add_library(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(SFML REQUIRED COMPONENTS graphics window system)
find_package(assimp REQUIRED)


add_subdirectory(entities)
add_subdirectory(file-import)
add_subdirectory(project-exceptions)
