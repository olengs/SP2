#pragma once
#ifndef MAIN_MENU_CAMERA_H
#define MAIN_MENU_CAMERA_H
#include "Camera.h"

class MainMenuCamera : public Camera
{

public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	MainMenuCamera();
	~MainMenuCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

};

#endif