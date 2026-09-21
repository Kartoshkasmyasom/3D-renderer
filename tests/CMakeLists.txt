cmake_minimum_required(VERSION 3.10)

project(renderer-tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(renderer_tests test_main.cpp)

target_include_directories(
  renderer_tests
  PRIVATE ${CMAKE_SOURCE_DIR}/app/include
          ${CMAKE_SOURCE_DIR}/world/include
          ${CMAKE_SOURCE_DIR}/user-dialogue/include)

target_link_libraries(renderer_tests PRIVATE user-dialogue world)

add_test(NAME renderer_tests COMMAND renderer_tests)
