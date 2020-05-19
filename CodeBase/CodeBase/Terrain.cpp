#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"


Terrain::Terrain(GameObject* _parent, int nChunks)
{
	parent = _parent;
	GenerateChunks(nChunks, height, width);
}

Terrain::~Terrain()
{
	void DeleteChunks();

	parent = nullptr;
}

void Terrain::DrawChunks()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->mesh->DrawMesh(chunks[i]->selected);
	}
}

void Terrain::DrawSelectionChunks()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->mesh->DrawSelectionMesh();
	}
}

void Terrain::GenerateChunks(int nChunks, float height, float width)
{
	numChunks = nChunks;					//Side
	totalkChunks = nChunks * nChunks;		//Full grid

	for (int i = 0; i < totalkChunks; ++i)
	{
		int x = i / numChunks;
		int y = i % numChunks;
		float color = (float)i / float(totalkChunks);
		Chunk* curr = new Chunk(parent, i, x, y, height,width,color);
		chunks.push_back(curr);
	}
}

void Terrain::DeleteChunks()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		delete chunks[i];
		chunks[i] = nullptr;
	}

	totalkChunks = 0;
	chunks.clear();
}

