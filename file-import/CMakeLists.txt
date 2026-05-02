cmake_minimum_required(VERSION 3.10)

project(file-import)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(${PROJECT_NAME} src/ImportMesh.cpp ImportObj.cpp)       
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/entities/include    
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    entities
    glfw
    assimp
    sfml-graphics
    sfml-window
    sfml-system
)