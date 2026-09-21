cmake_minimum_required(VERSION 3.10)

project(renderer)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
            include/Renderer.hpp
            include/Shader.hpp
            src/Renderer.cpp
            src/Shader.cpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC world)
