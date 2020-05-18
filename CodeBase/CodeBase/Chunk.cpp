#include "Application.h"
#include "Chunk.h"
#include "GameObject.h"
#include "Mesh.h"

Chunk::Chunk(GameObject* _parent, int id, int _x, int _y, float height, float width)
{
	mesh = new Mesh(_parent,_x,_y, height, width);
	chunkID = id;
	x = _x;
	y = _y;
}

Chunk::~Chunk()
{
	delete mesh;
	mesh = nullptr;

	parent = nullptr;
}
