#pragma once
#include <glad/glad.h> // included just so the glfw include doesn't complain...
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Camera
{
private:
    double lastX = 0.0f;
    double lastY = 0.0f;
    bool rotating = false;
    float xRotation = 0.0f;
    float yRotation = 0.0f;

public:
    // fixed:
    vec3 cameraInitialPos;
    // updated when moving/rotating:
    vec3 cameraPos; // important
    vec3 cameraTarget; // important - or not? NOT ANYMORE REMOVE
    
    // used for moving?:
    vec3 up;
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
    void rotationMouse(GLFWwindow* window);
    void translate(glm::vec3 translation);
    void rotate(glm::vec3 axis, float radians);
    void moveForward(float howMuch);
    glm::vec3 getForward();
    glm::vec3 getRight(); 
    //glm::vec3 getUp();
    //glm::vec3 calculateDirections(glm::vec3 front, glm::vec3 up, glm::vec3 right);
};

