//
// Created by Luka on 09.12.2025.
//

#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"

class InputHandler
{
public:
    Camera* camera;
    float lastX, lastY;
    bool firstMouse;

    InputHandler(Camera* cam, float width, float height);

    void ProcessKeyboard(GLFWwindow* window, float deltaTime) const;
    void MouseCallback(GLFWwindow* window, double xpos, double ypos);

    void ProcessInput(GLFWwindow * window, float delta_time);
};