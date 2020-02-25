#pragma once
struct cornercoord {
	float minX, minY, minZ, maxX, maxY, maxZ;
	cornercoord(float minX = 0, float minY = 0, float minZ = 0, float maxX = 0, float maxY = 0, float maxZ = 0)
	{ 
		Set(minX, minY, minZ, maxX, maxY, maxZ);
	}
	void Set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	{ 
		this->minX = minX; this->minY = minY; this->minZ = minZ;
		this->maxX = maxX; this->maxY = maxY; this->maxZ = maxZ;
	}
};