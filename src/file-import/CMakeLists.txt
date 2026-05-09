cmake_minimum_required(VERSION 3.10)

project(file-import)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} src/Importer.cpp include/Importer.hpp)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_SOURCE_DIR}/entities/include)

target_link_libraries(${PROJECT_NAME} PUBLIC assimp::assimp entities)
