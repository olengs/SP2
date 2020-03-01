#pragma once
class cornercoord {
private:
	float minX, minY, minZ, maxX, maxY, maxZ;

public:
	cornercoord(float minX = 0, float minY = 0, float minZ = 0, float maxX = 0, float maxY = 0, float maxZ = 0)
	{ 
		Set(minX, minY, minZ, maxX, maxY, maxZ);
	}
	void Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	{
		this->minX = minX; this->minY = minY; this->minZ = minZ;
		this->maxX = maxX; this->maxY = maxY; this->maxZ = maxZ;
	}
	float getminX();
	float getminY();
	float getminZ();
	float getmaxX();
	float getmaxY();
	float getmaxZ();
	void setminX(float newvar);
	void setminY(float newvar);
	void setminZ(float newvar);
	void setmaxX(float newvar);
	void setmaxY(float newvar);
	void setmaxZ(float newvar);
};