#ifndef TRS_H
#define TRS_H
#include "Vector3.h"

struct Vector4 {
	float degree, x, y, z;
	Vector4(float degree = 0, float x = 0, float y = 0, float z = 0) { Set(degree, x, y, z); }
	void Set(float degree, float x, float y, float z) { this->degree = degree; this->x = x; this->y = y; this->z = z; }
	Vector4 operator+(const Vector4& Rvalue);//overloaded operator for Vector4 + Vector4
	Vector4 operator-(const Vector4& Rvalue);//overloaded operator for Vector4 - Vector4
	Vector4& operator+=(const Vector4& Rvalue);//overloaded operator for Vector4 += Vector4
	Vector4& operator-=(const Vector4& Rvalue);//overloaded operator for Vector4 -= Vector4
};

struct TRS {
	Vector3 translate;
	Vector4 RotateX = Vector4(0, 1, 0, 0);
	Vector4 RotateY = Vector4(0, 0, 1, 0);
	Vector4 RotateZ = Vector4(0, 0, 0, 1);
	Vector3 Scale = Vector3(1, 1, 1);
	TRS operator+(TRS& rValue);
};
#endif // !TRS_H
