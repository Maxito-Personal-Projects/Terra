#ifndef __MESH_H__
#define __MESH_H__

#include <string>
#include <map>

class GameObject;
class Chunk;

class Mesh
{
public:

	Mesh(GameObject* _parent, Chunk* _chunkP, int x, int y, float _height, float _width, float color);
	~Mesh();

	void DrawMesh(bool updateTFB, bool selected);
	void DrawSelectionMesh();
	void GenerateFlatMesh_quads(int x, int y);
	void LoadToGPU();
	void FillInfoGPU();

	void GenerateVertexBuffer();

	void DrawTextureToExport(string path);

public:

	float* vertices = nullptr;
	int* indices = nullptr;
	float* infoGPU = nullptr;

	int numVertices = 0;
	int numIndices = 0;

	uint VBO;	//Vertex Buffer Object
	uint VAO;	//Vertex Array Object
	uint IBO;	//Index Buffer Object
	uint TBO;	//Transform Feedback Buffer Object
	uint TBO_mid;	//Transform Feedback Buffer Object
	uint TBO_low;	//Transform Feedback Buffer Object

	uint VAOrender;	//Vertex Buffer Object

	float* vertexBuffer64 = nullptr;
	int buffSize64 = 0;
	float* vertexBuffer32 = nullptr;
	int buffSize32 = 0;
	float* vertexBuffer16 = nullptr;
	int buffSize16 = 0;

	GameObject* parent = nullptr;
	Chunk* chunkParent = nullptr;

	int size = 2;
	float width = 64.0f;
	float height = 64.0f;
	float divisions = 64.0f;

	int chunkX = 0;
	int chunkY = 0;

	float time = 0.0f;
	float3 selectColor = { 0.0f,0.0f,0.0f };

};

#endif //!__MESH_H__