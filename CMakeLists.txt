cmake_minimum_required(VERSION 3.10)
project(OpenGLGame)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Window.cpp
    src/Shader.cpp
    libs/glad/src/glad.c
)

include_directories(
    libs/glfw/include
    libs/glad/include
    src
)

add_subdirectory(libs/glfw)
add_library(glad STATIC libs/glad/src/glad.c)

add_executable(OpenGLGame ${SOURCES})
target_link_libraries(OpenGLGame glfw glad)
