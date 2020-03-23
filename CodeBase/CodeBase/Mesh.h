#ifndef _MESH_H__
#define _MESH_H__

#include <string>

class Mesh
{
public:

	Mesh();
	~Mesh();

	void DrawMesh();
	void GenerateFlatMesh();
	void LoadToGPU();

public:

	float vertices[12] = { 0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,-0.5f, -0.5f, 0.0f,-0.5f,  0.5f, 0.0f };
	int indices[6] = { 0,1,3,1,2,3 };
	int numVertices = 4;
	int numIndices = 6;

	uint VBO;
	uint VAO;
	uint IBO;

};

#endif //!_MESH_H__