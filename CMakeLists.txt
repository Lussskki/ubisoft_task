# 1. Set minimum required version (3.10 is a good modern minimum)
cmake_minimum_required(VERSION 3.10)

# 2. Define the project
project(ubisoft_task LANGUAGES CXX)

# Set C++ Standard (14 is fine, but 17 is common now)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 3. Find the GLFW package (vcpkg makes this available)
find_package(glfw3 CONFIG REQUIRED)

# 4. Define the executable, listing ONLY source files
add_executable(
        ubisoft_task
        src/main.cpp
)

# 5. CRITICAL: Link the GLFW library to the executable
# 'glfw' is the target name provided by vcpkg after find_package(glfw3)
target_link_libraries(
        ubisoft_task
        PRIVATE
        glfw
        OpenGL32
)