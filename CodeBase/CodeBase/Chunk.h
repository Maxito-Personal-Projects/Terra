#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "MathGeoLib/Math/float3.h"

class GameObject;
class Mesh;

class Chunk
{
public:
	
	Chunk(GameObject* _parent, int id, int _x, int _y, float height, float width,float _color);

	~Chunk();

	void SetNeighbours();

public:

	GameObject* parent = nullptr;
	Mesh* mesh = nullptr;
	Chunk* neighbours[8];

	float maxHeight = 0.0f;
	int octaves = 8;

	int chunkID = 0;
	int x=0;
	int y=0;
	int numNeighbours = 0;

	float3 color = { 0.0f,0.0f,0.0f };
	bool selected = false;

};

#endif // !__CHUNK_H__
