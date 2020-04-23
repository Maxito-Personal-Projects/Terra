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
	void GenerateFlatMesh_tris();
	void GenerateFlatMesh_quads();
	void LoadToGPU();
	//void CalculateNormals();
	void CalculateVertexNormals();
	void FillInfoGPU();

public:

	float* vertices = nullptr;
	int* testIndices = nullptr;
	int* tileCoords = nullptr;

	float tesVertices[44] = { -0.5f, -0.5f, 0.5f,0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,
		 0.5f, -0.5f, 0.5f,0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,
		-0.5f, 0.5f, 0.5f,0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,
		 0.5f, 0.5f, 0.5f,0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f };

	//float* faceNormals = nullptr;
	float* infoGPU = nullptr;

	std::map<int, vec> vertexNormals;

	int indices[4] = {0,1,3,2};
	int numVertices = 8;
	int numIndices = 36;
	int numProperties = 3; //pos,color,normal

	uint VBO;	//Vertex Buffer Object
	uint VAO;	//Vertex Array Object
	uint IBO;	//Index Buffer Object
	uint TBO;	//Transform Feedback Buffer Object

	GameObject* parent = nullptr;

	int size = 2;
	float width = 64.0f;
	float height = 64.0f;
	float divisions = 64.0f;

	float time = 0.0f;

};

#endif //!_MESH_H__