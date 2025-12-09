cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(OpenGLProject src/main.cpp
        src/shaders/shader.frag
        src/shaders/shader.vert
        src/source_files/camera.cpp
        src/headers/camera.h
        src/source_files/input_handler.cpp
        src/headers/input_handler.h
        src/source_files/utils.cpp
        src/headers/utils.h
        src/headers/cube.h
        src/source_files/cube.cpp
)

target_link_libraries(OpenGLProject PRIVATE glfw GLEW::GLEW OpenGL::GL)
# Shader location
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

