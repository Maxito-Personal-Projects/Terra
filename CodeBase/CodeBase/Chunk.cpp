#include "Application.h"
#include "Terrain.h"
#include "Chunk.h"
#include "GameObject.h"
#include "Mesh.h"

Chunk::Chunk(GameObject* _parent, int id, int _x, int _y, float height, float width,float _color)
{
	parent = _parent;

	mesh = new Mesh(_parent,this,_x,_y, height, width,_color);
	chunkID = id;
	x = _x;
	y = _y;

	color = float3(_color,_color,1.0f);
}


Chunk::~Chunk()
{
	delete mesh;
	mesh = nullptr;

	for (int i = 0; i < 8; ++i)
	{
		if (neighbours[i])
		{
			neighbours[i] = nullptr;
		}
	}

	parent = nullptr;
}

void Chunk::SetNeighbours()
{
	Terrain* terrain = parent->terrain;

	int size = terrain->numChunks;

	neighbours[0] = terrain->GetChunkFromCoords(x - 1, y - 1, numNeighbours);
	neighbours[1] = terrain->GetChunkFromCoords(x, y - 1, numNeighbours);
	neighbours[2] = terrain->GetChunkFromCoords(x + 1 , y - 1, numNeighbours);
	neighbours[3] = terrain->GetChunkFromCoords(x + 1, y, numNeighbours);
	neighbours[4] = terrain->GetChunkFromCoords(x + 1, y + 1, numNeighbours);
	neighbours[5] = terrain->GetChunkFromCoords(x , y + 1, numNeighbours);
	neighbours[6] = terrain->GetChunkFromCoords(x - 1, y + 1, numNeighbours);
	neighbours[7] = terrain->GetChunkFromCoords(x - 1, y, numNeighbours);
}
