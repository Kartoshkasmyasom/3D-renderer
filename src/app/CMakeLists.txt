cmake_minimum_required(VERSION 3.10)

project(app)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
            src/App.cpp
            include/App.hpp
            include/Action.hpp
            include/ActionTraits.hpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC renderer file-import user-dialogue SFML::Window
                             SFML::System)
