#pragma once

#include"Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraInitialPos = cameraPos;
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    printf("Camera created\n");
}

void Camera::calculateVectors() 
{
    cameraUp = glm::vec3(0, 1, 0);
    cameraRight = -glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraUp = -glm::normalize(glm::cross(cameraFront, cameraRight));
}

vec3 Camera::getPosition()
{
    return cameraPos;
}
void Camera::rotationMouse(GLFWwindow *window)
{
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!rotating) {
            rotating = true;
            lastX = xpos;
            lastY = ypos;
            return;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        //scale down the movement
        float sensitivity = 0.2f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        //left/right
        yRotation -= (float)xoffset;
        //up/down
        xRotation += (float)yoffset;

        //prevents from flipping upside down
        if (xRotation > 89.0f) {
            xRotation = 89.0f;
        }
        if (xRotation < -89.0f) {
            xRotation = -89.0f;
        }
        
        printf("pitch: %f heading: %f\n", xRotation, yRotation);
        cameraFront = -glm::vec3(sin(yRotation / 20), -sin(xRotation / 20), cos(xRotation / 20) * cos(yRotation / 20));
        calculateVectors();
    }
    else rotating = false;
}
void Camera ::translate(glm::vec3 translation)
{
    cameraPos = (up + getForward() + getRight()) * translation;
}
void Camera::rotate(glm::vec3 axis, float radians)
{
    cameraFront = glm::normalize(glm::rotate(cameraFront, radians, axis));
    cameraRight = glm::normalize(glm::cross(cameraFront, up));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::moveForward(float howMuch) 
{
    calculateVectors();
    cameraPos -= cameraFront * howMuch;
}

void Camera::moveRight(float howMuch) 
{
    calculateVectors();
    cameraPos += cameraRight * howMuch;
}

void Camera::moveUp(float howMuch)
{
    calculateVectors();
    cameraPos += cameraUp * howMuch;
}

vec3 Camera::getForward()
{
    return cameraFront;
}

vec3 Camera::getRight()
{
    glm::vec3 forward = getForward();
    return glm::normalize(glm::cross(forward, -up));
}

    

