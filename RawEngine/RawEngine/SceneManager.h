#pragma once
#include <glm/glm.hpp>
#include <vector> 
#include "GameObject.h"
#include "Camera.h"
#include <memory>
#include "Scene.h"
#include <unordered_map>
#include <string>

class SceneManager
{
public:
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::shared_ptr<Scene> currentScene = nullptr;
	 
		void CreateScene(string &name);
		void SetActiveScene(string &name);
		void AddScene(Scene scene, string name);
		void render();
};

