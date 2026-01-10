
#include "SceneManager.h"

void SceneManager::SetActiveScene(string &name)
{
	auto toActivate = scenes.find(name);
	if(toActivate != scenes.end())
		currentScene = toActivate->second;	
}
void SceneManager::CreateScene(string &name)
{

}
void SceneManager::AddScene(Scene scene, string name)
{
	//scenes.insert({name,scene});  ????
}
void SceneManager::render()
{
	//currentScene->renderScene();
}
