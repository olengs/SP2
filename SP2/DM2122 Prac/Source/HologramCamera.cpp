#include "HologramCamera.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

HologramCamera::HologramCamera()
{
}

HologramCamera::~HologramCamera()
{
}

void HologramCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	magnitude = Math::FAbs(target.z - pos.z);

}

void HologramCamera::Update(double dt, HologramUI CurrentHologram, TRS Player, float targetY)
{
	Player.RotateY.degree = 180.f;
	Mtx44 rotation;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	Vector3 actualup = Vector3(0, 1, 0);
	up = Vector3(0, 1, 0);

	target = CurrentHologram.UI.translate;
	position = CurrentHologram.UI.translate + Vector3(0, 0,sqrt(CurrentHologram.lengthX*CurrentHologram.lengthX + CurrentHologram.lengthY*CurrentHologram.lengthY));

	view = (target - position).Normalized();

//	if (Application::IsKeyPressed(VK_LEFT)) CurrentHologram.HologramTexture = 1;
//	else if (Application::IsKeyPressed(VK_RIGHT)) CurrentHologram.HologramTexture = 0;

}

void HologramCamera::Reset()
{
}
