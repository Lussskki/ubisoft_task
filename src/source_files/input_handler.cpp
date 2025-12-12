//
// Created by Luka on 09.12.2025.
//

#include "../headers/input_handler.h"
#include <iostream>

InputHandler::InputHandler(Camera* cam, float width, float height)
{
    camera = cam;
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    bloomEnabled = false; // Initialize the flag
}

void InputHandler::ProcessKeyboard(GLFWwindow* window, float deltaTime) const {
    // FIX: Pass the Camera_Movement enum value instead of the GLFW key code (int)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputHandler::ProcessInput(GLFWwindow* window, float deltaTime) {
    ProcessKeyboard(window, deltaTime);

    static bool bKeyPressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (!bKeyPressedLastFrame)
        {
            bloomEnabled = !bloomEnabled;
            std::cout << "Bloom Toggled: " << (bloomEnabled ? "ON" : "OFF") << std::endl;
        }
        bKeyPressedLastFrame = true;
    }
    else
    {
        bKeyPressedLastFrame = false;
    }
}