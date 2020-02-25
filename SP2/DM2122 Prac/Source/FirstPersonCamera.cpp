#include "FirstPersonCamera.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

FirstPersonCamera::FirstPersonCamera()
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void FirstPersonCamera::Update(double dt, TRS player)
{
	static const float CAMERA_SPEED = 10.f;
	static const float ROTATE_SPEED = 135.f;
	float playerheight = player.translate.y + 6.11;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	Vector3 actualup = Vector3(0, 1, 0);

	position = Vector3(player.translate.x, playerheight, player.translate.z);
	target = position + view;

	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(ROTATE_SPEED * dt);
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, actualup.x, actualup.y, actualup.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		up = right.Cross(view).Normalized();
		float yaw = (float)(-ROTATE_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, actualup.x, actualup.y, actualup.z);
		view = rotation * view;
		target = position + view;
	}

	//if (Application::IsKeyPressed(VK_UP))
	//{
	//	float pitch = (float)(ROTATE_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
	//if (Application::IsKeyPressed(VK_DOWN))
	//{
	//	float pitch = (float)(-ROTATE_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void FirstPersonCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}