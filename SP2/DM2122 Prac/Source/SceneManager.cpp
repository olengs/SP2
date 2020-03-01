#include "SceneManager.h"
SceneManager* SceneManager::instance = 0;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
	delete this->instance;
}

SceneManager* SceneManager::getInstance() {
	if (!instance) {
		instance = new SceneManager();
		return instance;
	}
	else {
		return instance;
	}
}

Scene* SceneManager::getCurrScene() {
	return Scenes[currSceneID];
}

void SceneManager::Update(double ElapsedTime) {
	Scenes[currSceneID]->Update(ElapsedTime);
	Scenes[currSceneID]->Render();
}

int SceneManager::getCurrSceneID() {
	return currSceneID;
}

void SceneManager::setCurrSceneID(int ID) {
	currSceneID = ID;
}