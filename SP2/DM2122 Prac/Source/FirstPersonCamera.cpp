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
	Mtx44 rotation;
	static const float CAMERA_SPEED = 10.f;
	static const float ROTATE_SPEED = 135.f;
	float playerheight = player.translate.y + 2;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	up = right.Cross(view).Normalized();
	Vector3 actualup = Vector3(0, 1, 0);
	rotation.SetToRotation(player.RotateY.degree, actualup.x, actualup.y, actualup.z);

	view = (target - position).Normalized();
	position = Vector3(player.translate.x, playerheight, player.translate.z);
	target = position + Vector3(sin(Math::DegreeToRadian(player.RotateY.degree)), 0, cos(Math::DegreeToRadian(player.RotateY.degree)));;
}

void FirstPersonCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void FirstPersonCamera::FPCCar(double dt, TRS Car)
{
	Mtx44 rotation;
	static const float CAMERA_SPEED = 10.f;
	static const float ROTATE_SPEED = 135.f;
	float carheight = Car.translate.y + 2;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	up = right.Cross(view).Normalized();
	Vector3 actualup = Vector3(0, 1, 0);
	rotation.SetToRotation(Car.RotateY.degree, actualup.x, actualup.y, actualup.z);

	view = (target - position).Normalized();
	position = Vector3(Car.translate.x, carheight, Car.translate.z);
	target = position + Vector3(sin(Math::DegreeToRadian(-Car.RotateY.degree)), 0, -cos(Math::DegreeToRadian(Car.RotateY.degree)));;

}