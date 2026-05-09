cmake_minimum_required(VERSION 3.10)

project(app)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
            src/App.cpp
            include/App.hpp
            ${CMAKE_SOURCE_DIR}/edit/commands/Commands.cpp
            ${CMAKE_SOURCE_DIR}/edit/validators/Validators.cpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                  ${CMAKE_SOURCE_DIR}/app-context
                                  ${CMAKE_SOURCE_DIR}/edit
                                  ${CMAKE_SOURCE_DIR}/edit/commands
                                  ${CMAKE_SOURCE_DIR}/edit/messages
                                  ${CMAKE_SOURCE_DIR}/edit/requests
                                  ${CMAKE_SOURCE_DIR}/edit/validators)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC entities file-import user-dialogue SFML::Window
                             SFML::System)
