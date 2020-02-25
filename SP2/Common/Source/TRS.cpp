#include "TRS.h"

Vector4 Vector4::operator+(const Vector4& Rvalue)
{
	return Vector4(this->degree + Rvalue.degree, this->x, this->y, this->z);
	
}

Vector4 Vector4::operator-(const Vector4& Rvalue)
{
	return Vector4(this->degree - Rvalue.degree, this->x, this->y, this->z);
}

Vector4& Vector4::operator+=(const Vector4& Rvalue)
{
	this->degree += Rvalue.degree;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& Rvalue)
{
	this->degree -= Rvalue.degree;
	return *this;
}

TRS TRS::operator+(TRS& rValue)
{
	TRS temp = *this;
	temp.translate += rValue.translate;
	return temp;
}
