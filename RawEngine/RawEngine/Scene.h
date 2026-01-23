#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector> 
#include "GameObject.h"
#include "Camera.h"
#include <memory>
#include "core/model.h"
#include <string>
using namespace std;


class Scene {
public:
    std::vector <core::Model*> objs;
    string name;
    Scene(string _name);
    void AddObj(core::Model* model);
    void renderScene(GLuint shader,
        const glm::mat4& view,
        const glm::mat4& projection,
        GLint mvpUniform,
        GLint modelUniform);
};