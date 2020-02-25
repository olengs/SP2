#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
private:
	//int nextSceneID = 0;
	static SceneManager* instance;
public:
	enum SceneNames {
		S_SHOWROOMSCENE = 0,
		S_DRIVESCENE,
		S_TOTAL
	};
	SceneManager();
	~SceneManager();
	static SceneNames currSceneID;
	Scene* Scenes[S_TOTAL];
	static SceneManager* getInstance();
	Scene* getCurrScene();
	void Update(double ElapsedTime);
	SceneNames getCurrSceneID();
	void setCurrSceneID(SceneNames ID);
};

