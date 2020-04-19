#include "GameObject.h"
#include "Terrain.h"



Terrain::Terrain(GameObject* _parent)
{
	parent = _parent;
}


Terrain::~Terrain()
{
	parent = nullptr;
}
