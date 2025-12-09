cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(OpenGLProject main.cpp
        shaders/shader.frag
        shaders/fragment.vert)

target_link_libraries(OpenGLProject PRIVATE glfw GLEW::GLEW OpenGL::GL)
