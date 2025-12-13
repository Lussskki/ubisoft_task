# UBISTOFT_TASK – Real-Time Bloom Renderer
# Project Description

UBISTOFT_TASK is a C++ OpenGL project that demonstrates a full HDR Bloom post-processing pipeline.
The project renders an HDR scene, extracts bright areas, applies a separable Gaussian blur using ping-pong framebuffers, and finally combines everything with tone mapping.

The renderer includes a custom camera system, shader utilities, and real-time input handling.

## Features

 - Full HDR scene rendering
 - Brightness extraction to create a “bright mask”
 - Two-pass Gaussian blur using ping-pong FBOs
 - Final bloom composition with exposure & tone mapping
 - Custom vertex and fragment shaders
 - FPS-style perspective camera
 - Input handler with bloom toggle (B key)
 - Efficient cube rendering with custom shaders

## Requirements
 - C++ compiler (C++14 or later)
 - OpenGL 3.3 Core Profile or higher
 - GLFW
 - CMake

## Project Structure
```
ubisoft_task/
            src/
              shaders/
                bloom_bright.frag
                bloom_final.frag
                blur.frag
                quad.vert
                shader.frag
                shader.vert
              headers/
                camera.h
                cube.h
                input_handler.h
                utils.h
              source_files/
                camera.cpp
                cube.cpp
                input_handler.cpp
                utils.cpp
                
                main.cpp
```

## YouTube Video

<a href="https://youtu.be/Qrr64sW5nDE" target="video">▶️ Watch on YouTube</a>
