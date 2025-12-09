//
// Created by Luka on 09.12.2025.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <GL/glew.h>


// Function declarations
std::string readFile(const std::string& path);
GLuint compileShader(GLenum type, const char* code);
GLuint createShaderProgram(const char* vertexCode, const char* fragmentCode);

#endif