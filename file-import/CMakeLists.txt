cmake_minimum_required(VERSION 3.10)

project(file-import)

#add_library(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen3 REQUIRED)

add_subdirectory(assimp)
add_executable(${PROJECT_NAME} ImportObj.cpp)       
target_link_libraries(${PROJECT_NAME} 
PRIVATE
OpenGL::GL
GLEW::GLEW
glfw
assimp
Eigen3::Eigen)