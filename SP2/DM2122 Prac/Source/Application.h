
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SceneManager.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

private:
	SceneManager* thisScene;
	//Declare a window object
	StopWatch m_timer;
	bool applicationsound;
};
#endif