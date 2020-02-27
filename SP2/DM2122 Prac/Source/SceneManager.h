#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
private:
	//int nextSceneID = 0;
	static SceneManager* instance;
	int currSceneID = 0;
public:
	enum SceneNames {
		S_MAINMENUSCENE = 0,
		S_SHOWROOMSCENE,
		S_DRIVESCENE,
		S_TOTAL
	};
	SceneManager();
	~SceneManager();
	Scene* Scenes[S_TOTAL];
	static SceneManager* getInstance();
	Scene* getCurrScene();
	void Update(double ElapsedTime);
	int getCurrSceneID();
	void setCurrSceneID(int ID);
};

