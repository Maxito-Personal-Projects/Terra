#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"


Terrain::Terrain(GameObject* _parent, int nChunks)
{
	parent = _parent;
	numChunks = nChunks; //Side

	totalkChunks = nChunks * nChunks; //Full grid

	for (int i = 0; i < totalkChunks; ++i)
	{
		int x = i / numChunks;
		int y = i % numChunks;
		Chunk* curr = new Chunk(_parent, i,x,y);
		chunks.push_back(curr);
	}
}

Terrain::~Terrain()
{
	parent = nullptr;
}

void Terrain::DrawChunks()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->mesh->DrawMesh();
	}
}

