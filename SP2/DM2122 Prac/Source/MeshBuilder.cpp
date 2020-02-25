#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// 3 lines using 6 vertices
	vertex_buffer_data.reserve(6);

	v.pos.Set(0 - lengthX * 0.5, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX * 0.5, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0 - lengthX * 0.5, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthX * 0.5, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 0 - lengthX * 0.5);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthX * 0.5);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.reserve(6);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0],
		GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string& meshName, Color color, float lengthX, float lengthY)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// a quad has 4 vertices
	vertex_buffer_data.reserve(4);
	v.color = color;
	v.normal.Set(0, 0, 1.0f);

	float midvalueX = lengthX * 0.5;
	float midvalueY = lengthY * 0.5;
	v.pos.Set(-midvalueX, midvalueY, 0);
	v.texCoord.Set(0, 1.f);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-midvalueX, -midvalueY, 0);
	v.texCoord.Set(0.f, 0.f);
	vertex_buffer_data.push_back(v);

	v.pos.Set(midvalueX, midvalueY, 0);
	v.texCoord.Set(1.f, 1.f);
	vertex_buffer_data.push_back(v);

	v.pos.Set(midvalueX, -midvalueY, 0);
	v.texCoord.Set(1.f, 0.f);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.reserve(6);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], 
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0],
		GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cuboid; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCuboid(const std::string& meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float midvalueX = lengthX * 0.5;
	float midvalueY = lengthY * 0.5;
	float midvalueZ = lengthZ * 0.5;
	v.color = color;

	//Top
	v.normal = Vector3(0.f, 1.f, 0.f);
	v.pos.Set(midvalueX, midvalueY, midvalueZ);
	v.texCoord.Set(0, 0);

	vertex_buffer_data.push_back(v); //0
	v.pos.Set(midvalueX, midvalueY, -midvalueZ);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v); //1
	v.pos.Set(-midvalueX, midvalueY, -midvalueZ);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v); //2
	v.pos.Set(-midvalueX, midvalueY, midvalueZ);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v); //3

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);

	//Rside
	v.normal = Vector3(1.f, 0.f, 0.f);
	v.pos.Set(midvalueX, -midvalueY, midvalueZ);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v); //4
	v.pos.Set(midvalueX, -midvalueY, -midvalueZ);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v); //5
	v.pos.Set(midvalueX, midvalueY, -midvalueZ);
	v.texCoord.Set(1,1);
	vertex_buffer_data.push_back(v); //6
	v.pos.Set(midvalueX, midvalueY, midvalueZ);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v); //7

	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);

	//Front
	v.normal = Vector3(0.f, 0.f, 1.f);
	v.pos.Set(midvalueX, midvalueY, midvalueZ);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v); //8
	v.pos.Set(-midvalueX, midvalueY, midvalueZ);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v); //8
	v.pos.Set(-midvalueX, -midvalueY, midvalueZ);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v); //10
	v.pos.Set(midvalueX, -midvalueY, midvalueZ);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v); //11

	index_buffer_data.push_back(8);
	index_buffer_data.push_back(9);
	index_buffer_data.push_back(10);
	index_buffer_data.push_back(10);
	index_buffer_data.push_back(11);
	index_buffer_data.push_back(8);

	//Back
	v.normal = Vector3(0.f, 0.f, -1.f);
	v.pos.Set(midvalueX, -midvalueY, -midvalueZ);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v); //12
	v.pos.Set(-midvalueX, -midvalueY, -midvalueZ);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v); //13
	v.pos.Set(-midvalueX, midvalueY, -midvalueZ);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v); //14
	v.pos.Set(midvalueX, midvalueY, -midvalueZ);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v); //15

	index_buffer_data.push_back(12);
	index_buffer_data.push_back(13);
	index_buffer_data.push_back(14);
	index_buffer_data.push_back(14);
	index_buffer_data.push_back(15);
	index_buffer_data.push_back(12);

	//Lside
	v.normal = Vector3(-1.f, 0.f, 0.f);
	v.pos.Set(-midvalueX, -midvalueY, -midvalueZ);
	vertex_buffer_data.push_back(v); //16
	v.pos.Set(-midvalueX, -midvalueY, midvalueZ);
	vertex_buffer_data.push_back(v); //17
	v.pos.Set(-midvalueX, midvalueY, midvalueZ);
	vertex_buffer_data.push_back(v); //18
	v.pos.Set(-midvalueX, midvalueY, -midvalueZ);
	vertex_buffer_data.push_back(v); //19

	index_buffer_data.push_back(16);
	index_buffer_data.push_back(17);
	index_buffer_data.push_back(18);
	index_buffer_data.push_back(18);
	index_buffer_data.push_back(19);
	index_buffer_data.push_back(16);

	//Bottom
	v.normal = Vector3(0.f, -1.f, 0.f);
	v.pos.Set(-midvalueX, -midvalueY, -midvalueZ);
	vertex_buffer_data.push_back(v); //20
	v.pos.Set(midvalueX, -midvalueY, -midvalueZ);
	vertex_buffer_data.push_back(v); //21
	v.pos.Set(midvalueX, -midvalueY, midvalueZ);
	vertex_buffer_data.push_back(v); //22
	v.pos.Set(-midvalueX, -midvalueY, midvalueZ);
	vertex_buffer_data.push_back(v); //23

	index_buffer_data.push_back(20);
	index_buffer_data.push_back(21);
	index_buffer_data.push_back(22);
	index_buffer_data.push_back(22);
	index_buffer_data.push_back(23);
	index_buffer_data.push_back(20);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}


/******************************************************************************/
/*!
 \brief
 Generate the vertices of a Sphere or Hemisphere; Use random color for each vertex
 Then generate the VBO/IBO and store them in Mesh object

 \param meshName - name of mesh
 \param numSlice - number of triangle used to draw
 \param numStack - number of layers used to draw
 \param radius -  radius of the circle

 \return Pointer to mesh storing VBO/IBO of cube
 */
 /******************************************************************************/

float sphereX(float phi, float theta) {
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta) {
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta) {
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

/******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(v.pos.x, v.pos.y, v.pos.z);
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerSlice = 360.f / numSlice;
	for (unsigned slice = 0; slice < numSlice; ++slice) {

		float theta = slice * degreePerSlice;
		/*v0*/	v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		/*v1*/	v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		float theta2 = (slice + 1) * degreePerSlice;
		/*v2*/	v.pos.Set(outerR * cos(Math::DegreeToRadian(theta2)), 0, outerR * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		/*v3*/	v.pos.Set(innerR * cos(Math::DegreeToRadian(theta2)), 0, innerR * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < numSlice; ++slice) {
		index_buffer_data.push_back(slice * 4 + 0);
		index_buffer_data.push_back(slice * 4 + 1);
		index_buffer_data.push_back(slice * 4 + 2);

		index_buffer_data.push_back(slice * 4 + 3);
		index_buffer_data.push_back(slice * 4 + 2);
		index_buffer_data.push_back(slice * 4 + 1);
	}
	Mesh* mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}


Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh* mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height)
{
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 180.0f / numStack;
	float degreePerSlice = 360.0f / numSlice;
	float stackHeight = height / numStack;
	for (unsigned stack = 0; stack < numStack + 1; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * cos(Math::DegreeToRadian(theta)) /*cylinderX(theta)*/, 
					-height / 2 + stack * stackHeight, radius * sin(Math::DegreeToRadian(theta)) /*cylinderZ(theta)*/);
			v.color = color;
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}

	}

	Mesh* mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x2, y2, z2);
			v.color = color;

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshname, const std::string& file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success) return NULL;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshname);
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numrow, unsigned numcol)
{
	Vertex v;
	v.normal.Set(0, 0, 1.0f);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	vertex_buffer_data.reserve(numrow * numcol * 4);
	index_buffer_data.reserve(numrow * numcol * 6);
	int nCnt = 0;

	float width = 1.f / numcol;
	float height = 1.f / numrow;
	for (unsigned j = 0; j < numrow; ++j) {
		for (unsigned i = 0; i < numcol; ++i) {
			//v.pos.Set(0, 1, 0);
			//v.texCoord.Set(0 , 1);
			//vertex_buffer_data.push_back(v);
			//v.pos.Set(0, 0, 0);
			//v.texCoord.Set(0,0);
			//vertex_buffer_data.push_back(v);
			//v.pos.Set(1, 1, 0);
			//v.texCoord.Set(1, 1);
			//vertex_buffer_data.push_back(v);
			//v.pos.Set(1, 0, 0);
			//v.texCoord.Set(1, 0);
			//vertex_buffer_data.push_back(v);

			v.pos.Set(0, 1, 0);
			v.texCoord.Set(width*i , height*(numrow-j));
			vertex_buffer_data.push_back(v);
			v.pos.Set(0, 0, 0);
			v.texCoord.Set(width * i, height * (numrow-j-1));
			vertex_buffer_data.push_back(v);
			v.pos.Set(1, 1, 0);
			v.texCoord.Set(width * (i+1), height * (numrow-j));
			vertex_buffer_data.push_back(v);
			v.pos.Set(1, 0, 0);
			v.texCoord.Set(width*(i+1) , height * (numrow-j-1));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(nCnt);
			index_buffer_data.push_back(nCnt + 1);
			index_buffer_data.push_back(nCnt + 2);
			index_buffer_data.push_back(nCnt + 2);
			index_buffer_data.push_back(nCnt + 1);
			index_buffer_data.push_back(nCnt + 3);
			nCnt += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
