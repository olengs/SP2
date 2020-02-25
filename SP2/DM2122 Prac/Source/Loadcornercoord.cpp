#include "Loadcornercoord.h"
#include <iostream>
#include <fstream>
#include <map>
#include <MyMath.h>

void Loadcoord(const char* file_path, cornercoord& coord)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
		return;
	}
	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 tempcoord;
			sscanf_s((buf + 2), "%f%f%f", &tempcoord.x, &tempcoord.y, &tempcoord.z);
			coord.Set(Math::Min(tempcoord.x, coord.minX), Math::Min(tempcoord.y, coord.minY), Math::Min(tempcoord.z, coord.minZ),
				Math::Max(tempcoord.x, coord.maxX), Math::Max(tempcoord.y, coord.maxY), Math::Max(tempcoord.z, coord.maxZ));
		}
	}
}


void Loadcoord(const char* file_path, float& radius)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
		return;
	}
	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 tempcoord;
			sscanf_s((buf + 2), "%f%f%f", &tempcoord.x, &tempcoord.y, &tempcoord.z);
			radius = Math::Max(radius, tempcoord.Length());
		}
	}
}

