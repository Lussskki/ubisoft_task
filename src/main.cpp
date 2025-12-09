//
// Created by Luka on 08.12.2025.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/cube.h"
#include "headers/camera.h"
#include "headers/input_handler.h"
#include "headers/utils.h"

#include <iostream>

constexpr unsigned int SCR_WIDTH  = 1000;
constexpr unsigned int SCR_HEIGHT = 600;

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGl Renderer", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // --- Camera and Input Setup ---
    Camera camera;
    InputHandler input(&camera, SCR_WIDTH, SCR_HEIGHT);
    glfwSetWindowUserPointer(window, &input);
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        static_cast<InputHandler*>(glfwGetWindowUserPointer(w))->MouseCallback(w, x, y);
    });
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const Cube cube(glm::vec3(-1.0f, 0.0f, -5.0f));

    const std::string vertexCode   = readFile("../shaders/shader.vert");
    const std::string fragmentCode = readFile("../shaders/shader.frag");
    const GLuint shaderProgram = createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());

    // Time tracking for smooth movement
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Process keyboard input ---
        input.ProcessInput(window, deltaTime);

        glUseProgram(shaderProgram);

        // --- Model matrix ---
        glm::mat4 modelCube = glm::translate(glm::mat4(1.0f), cube.position);
        const GLint locModel = glGetUniformLocation(shaderProgram, "model");
        if (locModel >= 0)
            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelCube));

        // --- View matrix from camera ---
        glm::mat4 view = camera.GetViewMatrix();
        GLint locView = glGetUniformLocation(shaderProgram, "view");
        if (locView >= 0)
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));

        // --- Projection matrix ---
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
            0.1f,
            100.0f
        );
        GLint locProj = glGetUniformLocation(shaderProgram, "projection");
        if (locProj >= 0)
            glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(projection));

        // --- Draw the cube ---
        const GLint locUseTex = glGetUniformLocation(shaderProgram, "useTexture"); // optional
        cube.Draw(shaderProgram, locModel, locUseTex);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
