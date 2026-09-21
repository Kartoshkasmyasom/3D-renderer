cmake_minimum_required(VERSION 3.10)

project(user-dialogue)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
            src/Dialogue.cpp
            src/Parser.cpp
            src/Validators.cpp
            src/Window.cpp
            include/Dialogue.hpp
            include/Parser.hpp
            include/Token.hpp
            include/Validators.hpp
            include/MessageTraits.hpp
            include/Window.hpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                  ${CMAKE_SOURCE_DIR}/app/include
                                  ${CMAKE_SOURCE_DIR}/world/include)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC OpenGL::GL GLEW::GLEW SFML::Graphics SFML::Window
                             SFML::System)
