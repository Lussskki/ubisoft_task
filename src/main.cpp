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
#include <array>

constexpr unsigned int SCR_WIDTH  = 1000;
constexpr unsigned int SCR_HEIGHT = 600;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // Using 4 vertices and GL_TRIANGLE_STRIP to draw the quad
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // Top-left
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Top-right
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    // CRUCIAL CHANGE: Use GL_TRIANGLE_STRIP and draw 4 vertices (two triangles)
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


int main() {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    Camera camera;
    InputHandler input(&camera, SCR_WIDTH, SCR_HEIGHT);
    glfwSetWindowUserPointer(window, &input);
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        static_cast<InputHandler*>(glfwGetWindowUserPointer(w))->MouseCallback(w, x, y);
    });
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const Cube cube(glm::vec3(0.0f, 0.0f, -5.0f));

    const GLuint sceneShader = createShaderProgram(readFile("../src/shaders/shader.vert").c_str(), readFile("../src/shaders/shader.frag").c_str());
    const GLuint blurShader  = createShaderProgram(readFile("../src/shaders/quad.vert").c_str(), readFile("../src/shaders/blur.frag").c_str());
    const GLuint finalShader = createShaderProgram(readFile("../src/shaders/quad.vert").c_str(), readFile("../src/shaders/bloom_final.frag").c_str());

    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "HDR Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Ping-pong Framebuffer not complete!" << std::endl;
    }

    float gaussianWeights[5] = {0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f};
    glUseProgram(blurShader);
    glUniform1fv(glGetUniformLocation(blurShader, "weight"), 5, gaussianWeights);

    glUseProgram(finalShader);
    glUniform1i(glGetUniformLocation(finalShader, "scene"), 0);
    glUniform1i(glGetUniformLocation(finalShader, "bloomBlur"), 1);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input.ProcessInput(window, deltaTime);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(sceneShader);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
            0.1f,
            100.0f
        );
        GLint locView = glGetUniformLocation(sceneShader, "view");
        GLint locProj = glGetUniformLocation(sceneShader, "projection");
        if (locView >= 0) glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
        if (locProj >= 0) glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(projection));

        GLint locIntensity = glGetUniformLocation(sceneShader, "intensity");
        if (locIntensity >= 0) glUniform1f(locIntensity, 5.0f);

        glm::mat4 modelCube = glm::translate(glm::mat4(1.0f), cube.position);
        GLint locModel = glGetUniformLocation(sceneShader, "model");
        GLint locUseTex = glGetUniformLocation(sceneShader, "useTexture");

        if (locModel >= 0) glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelCube));

        cube.Draw(sceneShader, locModel, locUseTex);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        glUseProgram(blurShader);
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            glUniform1i(glGetUniformLocation(blurShader, "horizontal"), horizontal);
            glBindTexture(
                GL_TEXTURE_2D,
                first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]
            );
            renderQuad();
            horizontal = !horizontal;
            if (first_iteration) first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glUseProgram(finalShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

        glUniform1i(glGetUniformLocation(finalShader, "bloomEnabled"), input.bloomEnabled);
        glUniform1f(glGetUniformLocation(finalShader, "exposure"), 1.0f);

        renderQuad();

        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &hdrFBO);
    glDeleteFramebuffers(2, pingpongFBO);
    glDeleteBuffers(1, &rboDepth);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteTextures(2, colorBuffers);
    glDeleteTextures(2, pingpongColorbuffers);

    glfwTerminate();
    return 0;
}