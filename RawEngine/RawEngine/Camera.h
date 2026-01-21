#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Camera
{
public:
    // fixed:
    vec3 cameraInitialPos;
    // updated when moving/rotating:
    vec3 cameraPos;
    vec3 cameraTarget;
    vec3 up;
    
    
   /*
   vec3 up;
   */
   vec3 cameraFront;
    vec3 cameraRight;
    vec3 cameraUp;

    Camera();
    // TODO:
    // getPosition()
    void calculateVectors();
    void moveRight(float howMuch);
    void moveUp(float howMuch);
    vec3 getPosition();
    void translate(glm::vec3 translation);
    void rotate(glm::vec3 axis, float radians);
    void moveForward(float howMuch);
    glm::vec3 getForward();
    glm::vec3 getRight(); 
    //glm::vec3 getUp();
    //glm::vec3 calculateDirections(glm::vec3 front, glm::vec3 up, glm::vec3 right);
};

