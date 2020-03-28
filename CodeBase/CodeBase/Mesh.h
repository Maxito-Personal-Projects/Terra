#ifndef _MESH_H__
#define _MESH_H__

#include <string>
#include <map>

class GameObject;

class Mesh
{
public:

	Mesh(GameObject* _parent);
	~Mesh();

	void DrawMesh();
	void GenerateFlatMesh();
	void LoadToGPU();
	//void CalculateNormals();
	void CalculateVertexNormals();
	void FillInfoGPU();

public:

	float* vertices = nullptr;
	int* testIndices = nullptr;
	int* tileCoords = nullptr;

	float verticesTest[24] = { -0.5f, -0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, 0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f };

	float colors[24] = { 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
					     0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
					     0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f};

	//float* faceNormals = nullptr;
	float* infoGPU = nullptr;

	std::map<int, vec> vertexNormals;

	int indices[36] = { 0,1,2,1,3,2,1,7,3,7,6,3,7,5,6,5,4,6,2,3,4,3,6,4,5,0,4,0,2,4,5,7,0,7,1,0 };
	int numVertices = 8;
	int numIndices = 36;
	int numProperties = 3; //pos,color,normal

	uint VBO;
	uint VAO;
	uint IBO;

	GameObject* parent = nullptr;

	int size = 50;
	float width = 0.5f;
	float height = 0.5f;

};

#endif //!_MESH_H__