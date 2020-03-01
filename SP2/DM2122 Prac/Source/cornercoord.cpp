#include "cornercoord.h"

float cornercoord::getminX()
{
	return this->minX;
}

float cornercoord::getminY()
{
	return minY;
}

float cornercoord::getminZ()
{
	return minZ;
}

float cornercoord::getmaxX()
{
	return maxX;
}

float cornercoord::getmaxY()
{
	return maxY;
}

float cornercoord::getmaxZ()
{
	return maxZ;
}

void cornercoord::setminX(float newvar)
{
	this->minX = newvar;
}

void cornercoord::setminY(float newvar)
{
	this->minY = newvar;
}

void cornercoord::setminZ(float newvar)
{
	this->minZ = newvar;
}

void cornercoord::setmaxX(float newvar)
{
	this->maxX = newvar;
}

void cornercoord::setmaxY(float newvar)
{
	this->maxY = newvar;
}

void cornercoord::setmaxZ(float newvar)
{
	this->maxZ = newvar;
}
