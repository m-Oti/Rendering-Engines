#include "GameObject.h"

//GameObject::GameObject()
//{
//	position = glm::vec3(0.0f, 0.0f, 10.0f);
//};

GameObject::GameObject(glm::vec3 position, std::string name) : position(position) {

}

void GameObject::translate(glm::vec3 translation) {}
void GameObject::rotate() {}
void GameObject::rename(std::string newName) {}