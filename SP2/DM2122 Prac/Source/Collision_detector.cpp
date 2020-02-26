#include "Collision_detector.h"
#include <iostream>
#include <Mtx44.h>

//rectangle vs rectangle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, Vector3 dimension_obj2)
{
	return pos_obj1.x + dimension_obj1.x / 2 >= pos_obj1.x - dimension_obj2.x / 2
		&& pos_obj1.x - dimension_obj1.x / 2 <= pos_obj2.x + dimension_obj2.x
		&& pos_obj1.z + dimension_obj1.z / 2 >= pos_obj1.z - dimension_obj2.z / 2
		&& pos_obj1.z - dimension_obj1.z / 2 <= pos_obj2.z + dimension_obj2.z;
}
//rectangle vs circle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, float radius)
{
	float x = Math::Max(pos_obj1.x - dimension_obj1.x / 2, Math::Min(pos_obj2.x, pos_obj1.x - dimension_obj1.x));
	float z = Math::Max(pos_obj1.z - dimension_obj1.z / 2, Math::Min(pos_obj2.z, pos_obj1.z - dimension_obj1.z));

	float shortestdist = sqrt((x - pos_obj2.x) * (x - pos_obj2.x) + (z - pos_obj2.z) * (z - pos_obj2.z));
	return shortestdist < radius;
}
//circle vs circle

bool collision_detector(Vector3 pos_obj1, float radius1, Vector3 pos_obj2, float radius2)
{
	float shortestdist = sqrt((pos_obj1.x - pos_obj2.x) * (pos_obj1.x - pos_obj2.x)
		+ (pos_obj1.y - pos_obj2.y) * (pos_obj1.y - pos_obj2.y)
		+ (pos_obj1.z - pos_obj2.z) * (pos_obj1.z - pos_obj2.z));
	return shortestdist < (radius1 + radius2);
}

bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, cornercoord dimensions_obj2)
{
	return obj1.translate.x + dimensions_obj1.maxX * obj1.Scale.x >= obj2.translate.x + dimensions_obj2.minX * obj2.Scale.x
		&& obj1.translate.x + dimensions_obj1.minX * obj1.Scale.x <= obj2.translate.x + dimensions_obj2.maxX * obj2.Scale.x
		&& obj1.translate.z + dimensions_obj1.maxZ * obj1.Scale.z >= obj2.translate.z + dimensions_obj2.minZ * obj2.Scale.z
		&& obj1.translate.z + dimensions_obj1.minZ * obj1.Scale.z <= obj2.translate.z + dimensions_obj2.maxZ * obj2.Scale.z;
}

bool collision_detector(TRS obj1, cornercoord dimension_obj1, TRS obj2, float radius)
{
	float x = Math::Max(obj1.translate.x + dimension_obj1.minX * obj1.Scale.x
		, Math::Min(obj2.translate.x, obj1.translate.x + dimension_obj1.maxX * obj1.Scale.x));
	float z = Math::Max(obj1.translate.z + dimension_obj1.minZ * obj1.Scale.z
		, Math::Min(obj2.translate.z, obj1.translate.z + dimension_obj1.maxZ));

	float shortestdist = sqrt((x - obj2.translate.x) * (x - obj2.translate.x) + (z - obj2.translate.z) * (z - obj2.translate.z));
	return shortestdist < radius * obj2.Scale.x;
}

bool collision_detector(TRS& obj1, cornercoord coords1, TRS& obj2, cornercoord coords2, bool SAT)
{
	Vector3 obj1_coords[4], obj2_coords[4];
	setcoords(obj1_coords, obj1, coords1);
	setcoords(obj2_coords, obj2, coords2);
	std::vector<Vector3> normals;
	normals.reserve(4);
	setnormals(normals, obj1, coords1);
	setnormals(normals, obj2, coords2);
	float obj1_prj[4], obj2_prj[4];
	float obj1_min, obj1_max, obj2_min, obj2_max;
	int num_normals = normals.size();
	for (int i = 0; i < num_normals; ++i) {
		getprojection(obj1_coords, normals[i], obj1_prj);
		getprojection(obj2_coords, normals[i], obj2_prj);
		getminmax(obj1_min, obj1_max, obj2_min, obj2_max, obj1_prj, obj2_prj);
		if (obj1_max < obj2_min || obj2_max < obj1_min) {
			return false;
		}
	}
	return true;
}

void setcoords(Vector3* coords, TRS obj, cornercoord coordinates)
{
	Mtx44 rotation;
	rotation.SetToRotation(obj.RotateY.degree, 0, 1, 0);
	coords[0].Set(coordinates.maxX, 0, coordinates.maxZ);
	coords[1].Set(coordinates.minX, 0, coordinates.maxZ);
	coords[2].Set(coordinates.maxX, 0, coordinates.minZ);
	coords[3].Set(coordinates.minX, 0, coordinates.minZ);
	for (int i = 0; i < 4; ++i) {
		coords[i] *= obj.Scale.x;
		coords[i] = rotation * coords[i];
		coords[i] += obj.translate;
	}
}

void setnormals(std::vector<Vector3>& normals, TRS obj, cornercoord coordinates)
{
	Vector3 temp1, temp2;
	Mtx44 rotation;
	temp1.Set(coordinates.maxX, 0, 0);
	temp2.Set(0, 0, coordinates.maxZ);
	rotation.SetToRotation(obj.RotateY.degree, 0, 1, 0);
	temp1 = rotation * temp1;
	temp2 = rotation * temp2;
	normals.push_back(temp1);
	normals.push_back(temp2);
}

void getprojection(Vector3* coords, Vector3& normal, float* proj)
{
	for (int i = 0; i < 4; ++i) {
		proj[i] = coords[i].Dot(normal);
	}
}

void getminmax(float& minA, float& maxA, float& minB, float& maxB, float* prj1, float* prj2)
{
	minA = Math::Min(Math::Min(Math::Min(prj1[0], prj1[1]), prj1[2]), prj1[3]);
	maxA = Math::Max(Math::Max(Math::Max(prj1[0], prj1[1]), prj1[2]), prj1[3]);
	minB = Math::Min(Math::Min(Math::Min(prj2[0], prj2[1]), prj2[2]), prj2[3]);
	maxB = Math::Max(Math::Max(Math::Max(prj2[0], prj2[1]), prj2[2]), prj2[3]);
}
