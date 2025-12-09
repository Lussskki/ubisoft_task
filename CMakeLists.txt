cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(OpenGLProject src/main.cpp
        src/shaders/shader.frag
        src/shaders/shader.vert
        src/camera.cpp
        src/headers/camera.h
        src/input_handler.cpp
        src/headers/input_handler.h
        src/utils.cpp
        src/headers/utils.h
        src/headers/cube.h
        src/cube.cpp
)

target_link_libraries(OpenGLProject PRIVATE glfw GLEW::GLEW OpenGL::GL)
# Shader location
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

