#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateCuboid(const std::string& meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateRing(const std::string& meshName, Color color, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numrow, unsigned numcol);
};

#endif