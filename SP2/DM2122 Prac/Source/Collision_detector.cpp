#include "Collision_detector.h"
#include <iostream>
#include <Mtx44.h>

bool collision_detection::collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, cornercoord dimensions_obj2)
{
	return obj1.translate.x + dimensions_obj1.getmaxX() * obj1.Scale.x >= obj2.translate.x + dimensions_obj2.getminX() * obj2.Scale.x
		&& obj1.translate.x + dimensions_obj1.getminX() * obj1.Scale.x <= obj2.translate.x + dimensions_obj2.getmaxX() * obj2.Scale.x
		&& obj1.translate.z + dimensions_obj1.getmaxZ() * obj1.Scale.z >= obj2.translate.z + dimensions_obj2.getminZ() * obj2.Scale.z
		&& obj1.translate.z + dimensions_obj1.getminZ() * obj1.Scale.z <= obj2.translate.z + dimensions_obj2.getmaxZ() * obj2.Scale.z;
}

bool collision_detection::collision_detector(TRS obj1, cornercoord dimension_obj1, TRS obj2, float radius)
{
	float x = Math::Max(obj1.translate.x + dimension_obj1.getminX() * obj1.Scale.x
		, Math::Min(obj2.translate.x, obj1.translate.x + dimension_obj1.getmaxX() * obj1.Scale.x));
	float z = Math::Max(obj1.translate.z + dimension_obj1.getminZ() * obj1.Scale.z
		, Math::Min(obj2.translate.z, obj1.translate.z + dimension_obj1.getmaxZ()));

	float shortestdist = sqrt((x - obj2.translate.x) * (x - obj2.translate.x) + (z - obj2.translate.z) * (z - obj2.translate.z));
	return shortestdist < radius * obj2.Scale.x;
}

bool collision_detection::collision_detector(TRS& obj1, cornercoord coords1, TRS& obj2, cornercoord coords2, bool SAT)
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

void collision_detection::setcoords(Vector3* coords, TRS obj, cornercoord coordinates)
{
	Mtx44 rotation;
	rotation.SetToRotation(obj.RotateY.degree, 0, 1, 0);
	coords[0].Set(coordinates.getmaxX(), 0, coordinates.getmaxZ());
	coords[1].Set(coordinates.getminX(), 0, coordinates.getmaxZ());
	coords[2].Set(coordinates.getmaxX(), 0, coordinates.getminZ());
	coords[3].Set(coordinates.getminX(), 0, coordinates.getminZ());
	for (int i = 0; i < 4; ++i) {
		coords[i] *= obj.Scale.x;
		coords[i] = rotation * coords[i];
		coords[i] += obj.translate;
	}
}

void collision_detection::setnormals(std::vector<Vector3>& normals, TRS obj, cornercoord coordinates)
{
	Vector3 temp1, temp2;
	Mtx44 rotation;
	temp1.Set(coordinates.getmaxX(), 0, 0);
	temp2.Set(0, 0, coordinates.getmaxZ());
	rotation.SetToRotation(obj.RotateY.degree, 0, 1, 0);
	temp1 = rotation * temp1;
	temp2 = rotation * temp2;
	normals.push_back(temp1);
	normals.push_back(temp2);
}

void collision_detection::getprojection(Vector3* coords, Vector3& normal, float* proj)
{
	for (int i = 0; i < 4; ++i) {
		proj[i] = coords[i].Dot(normal);
	}
}

void collision_detection::getminmax(float& minA, float& maxA, float& minB, float& maxB, float* prj1, float* prj2)
{
	minA = Math::Min(Math::Min(Math::Min(prj1[0], prj1[1]), prj1[2]), prj1[3]);
	maxA = Math::Max(Math::Max(Math::Max(prj1[0], prj1[1]), prj1[2]), prj1[3]);
	minB = Math::Min(Math::Min(Math::Min(prj2[0], prj2[1]), prj2[2]), prj2[3]);
	maxB = Math::Max(Math::Max(Math::Max(prj2[0], prj2[1]), prj2[2]), prj2[3]);
}