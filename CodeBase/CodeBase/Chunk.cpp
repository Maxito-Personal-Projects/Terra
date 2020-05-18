#include "Application.h"
#include "Terrain.h"
#include "Chunk.h"
#include "GameObject.h"
#include "Mesh.h"

Chunk::Chunk(GameObject* _parent, int id, int _x, int _y, float height, float width,float _color)
{
	parent = _parent;

	mesh = new Mesh(_parent,_x,_y, height, width,_color);
	chunkID = id;
	x = _x;
	y = _y;

	color = float3(_color,_color,1.0f);
}


Chunk::~Chunk()
{
	delete mesh;
	mesh = nullptr;

	parent = nullptr;
}
