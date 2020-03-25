#ifndef _MESH_H__
#define _MESH_H__

#include <string>

class GameObject;

class Mesh
{
public:

	Mesh(GameObject* _parent);
	~Mesh();

	void DrawMesh();
	void GenerateFlatMesh();
	void LoadToGPU();

public:

	float vertices[48] = 
	{ -0.5f, -0.5f, 0.5f,0.0f,0.0f,0.0f,
		0.5f, -0.5f, 0.5f,0.0f,0.0f,0.0f,
		-0.5f, 0.5f, 0.5f,0.0f,0.0f,0.0f,
		0.5f, 0.5f, 0.5f,0.0f,0.0f,0.0f,
		-0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f,
		0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f,
		-0.5f, 0.5f, -0.5f,0.0f,0.0f,0.0f,
		0.5f, 0.5f, -0.5f,0.0f,0.0f,0.0f};

	int indices[36] = { 0,1,3,1,2,3,1,7,3,7,6,3,7,5,6,5,4,6,2,3,4,3,6,4,5,0,4,0,2,4,5,7,0,7,1,0 };
	int numVertices = 8;
	int numIndices = 36;

	uint VBO;
	uint VAO;
	uint IBO;

	GameObject* parent = nullptr;

};

#endif //!_MESH_H__