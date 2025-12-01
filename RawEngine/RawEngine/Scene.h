#pragma once
#include <glm/glm.hpp>
#include <vector> 
#include "GameObject.h"
#include "Camera.h"
using namespace std;


class Scene {
public:
    vector<GameObject*> objs;
    Camera Cam;

    Scene();
    
    void renderScene();


        //Scene()

        //create a render method in the gameobj class


        //void render();

};