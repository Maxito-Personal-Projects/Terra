#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Biome.h"


Terrain::Terrain(GameObject* _parent, int nChunks)
{
	parent = _parent;

	Biome* defaultBiome = new Biome("Default");
	biomes.push_back(defaultBiome);

	GenerateChunks(nChunks, height, width);

}

Terrain::~Terrain()
{
	void DeleteChunks();

	parent = nullptr;
}

void Terrain::DrawChunks(bool updateTFB)
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->mesh->DrawMesh(updateTFB, chunks[i]->selected);
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
		int x = i % numChunks;
		int y = i / numChunks;
		float color = (float)i / float(totalkChunks);
		Chunk* curr = new Chunk(parent, i, x, y, height,width,color,biomes[0]);
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

void Terrain::AddBiome(string name)
{
	Biome* newBiome = new Biome(name);
	biomes.push_back(newBiome);
}

void Terrain::DeleteBiome()
{
}

Chunk * Terrain::GetChunkFromCoords(int x, int y, int &num)
{
	Chunk* ret = nullptr;

	if (x>-1 && y>-1 && x<numChunks && y<numChunks)
	{
		int id = numChunks * y + x;
		num++;
		ret = chunks[id];
	}

	return ret;
}

void Terrain::SetNeighbours()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->SetNeighbours();
	}
}

