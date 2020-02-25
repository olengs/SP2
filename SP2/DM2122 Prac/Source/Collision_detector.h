#pragma once
#include <Vector3.h>
#include <TRS.h>
#include "cornercoord.h"
#include <vector>

bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, Vector3 dimension_obj2); //rectangle vs rectangle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, float radius); //rectangle vs circle
bool collision_detector(Vector3 pos_obj1, float radius1, Vector3 pos_obj2, float radius2); //circle vs circle

bool collision_detector(TRS obj1, cornercoord pos_obj1, TRS obj2, cornercoord pos_obj2);
bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, float radius);
bool collision_detector(TRS& obj1, cornercoord coords1, TRS& obj2, cornercoord coords2, bool SAT);

void setcoords(Vector3* coords, TRS obj, cornercoord coordinates);
void setnormals(std::vector<Vector3>& normals, TRS obj, cornercoord coordinates);
void getprojection(Vector3* coords, Vector3& normal, float* proj);
void getminmax(float& minA, float& maxA, float& minB, float& maxB, float* prj1, float* prj2);