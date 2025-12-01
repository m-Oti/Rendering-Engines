#pragma once

#include"Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraInitialPos = cameraPos;
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //cameraFront = glm::normalize(cameraPos - cameraTarget);
    //cameraRight = glm::vec3(1, 0, 0);
    //cameraUp = glm::vec3(0, 1, 0);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    //cameraRight = glm::normalize(glm::cross(up, cameraFront));
    //cameraUp = glm::cross(cameraFront, cameraRight);
    printf("Camera created\n");
}

void Camera::calculateVectors() {
    cameraFront = glm::normalize(cameraPos - cameraTarget);
    cameraUp = glm::vec3(0, 1, 0);
    cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
}

vec3 Camera::getPosition()
{
    return cameraPos;
}
void Camera ::translate(glm::vec3 translation)
{
    cameraPos = (up + getForward() + getRight()) * translation;
    cameraTarget = (up + getForward() + getRight()) * translation;
    /*cameraPos += translation;
    cameraTarget += translation;*/
}
void Camera::rotate(glm::vec3 axis, float radians)
{
    glm::vec3 currentPos = getPosition();
    cameraPos = cameraInitialPos;
    cameraPos = glm::rotate(cameraPos, radians, axis);
    cameraTarget = glm::rotate(cameraTarget, radians, axis);
    cameraPos = currentPos;

}

void Camera::moveForward(float howMuch) {
    calculateVectors();
    cameraPos += cameraFront * howMuch;
    cameraTarget += cameraFront * howMuch;
}

void Camera::moveRight(float howMuch) {
    calculateVectors();
    cameraPos += cameraRight * howMuch;
    cameraTarget += cameraRight * howMuch;
}

vec3 Camera::getForward()
{
    return glm::normalize(cameraTarget - cameraPos);
}

vec3 Camera::getRight()
{
    //return glm::normalize(glm::cross(getUp(), getForward()));
    glm::vec3 forward = getForward();
    return glm::normalize(glm::cross(forward, -up));
}

//vec3 Camera::getUp()
//{
//    //return glm::cross(getForward(), getRight());
//}

//vec3 Camera::calculateDirections(glm::vec3 front, glm::vec3 up, glm::vec3 right)
//{
//    right = getForward();
//
//}
    

