#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class GameObject
{
	//transform, roation, position and scaling,
	//model matrix
	//read uity documentation what game obj class has

	public:
		glm::vec3 position;
		std::string name;

		GameObject(glm::vec3 position, std::string name);

		void translate(glm::vec3 translation);
		void rotate();
		void rename(std::string newName);
};

