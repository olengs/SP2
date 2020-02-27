#ifndef SCENE_H
#define SCENE_H
#include "PlayerDetails.h"
class Scene
{
private:

public:
	Scene() {}
	~Scene() {}
	PlayerDetails playerdetails;
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	bool playsound;
	bool scenechange;
	int scenenumber;
};

#endif