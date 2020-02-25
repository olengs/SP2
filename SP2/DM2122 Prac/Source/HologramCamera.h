#pragma once
#include "Camera.h"
#include "HologramUI.h"

class HologramCamera :
	public Camera
{
public:

	HologramCamera();
	~HologramCamera();

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float magnitude;


	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt, HologramUI CurrentHologram, TRS Player, float targetY);
	virtual void Reset();
};

