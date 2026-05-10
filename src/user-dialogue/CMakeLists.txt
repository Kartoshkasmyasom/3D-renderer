cmake_minimum_required(VERSION 3.10)

project(user-dialogue)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
            src/Dialogue.cpp
            src/Parser.cpp
            include/Dialogue.hpp
            include/Parser.hpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                  ${CMAKE_SOURCE_DIR}/entities/include)
