#include "SceneManager.h"
SceneManager* SceneManager::instance = 0;
SceneManager::SceneNames SceneManager::currSceneID = SceneManager::S_SHOWROOMSCENE;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

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

SceneManager::SceneNames SceneManager::getCurrSceneID() {
	return currSceneID;
}

void SceneManager::setCurrSceneID(SceneNames ID) {
	currSceneID = ID;
}