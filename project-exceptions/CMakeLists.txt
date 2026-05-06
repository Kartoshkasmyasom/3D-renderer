cmake_minimum_required(VERSION 3.10)

project(project-exceptions)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} include/GlCheck.hpp src/ClCheck.cpp)        

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include  
)


target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    OpenGL::GL
    GLEW::GLEW
)
