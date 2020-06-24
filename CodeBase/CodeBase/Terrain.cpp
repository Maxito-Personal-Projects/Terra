#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Biome.h"
#include "ModuleUI.h"
#include "UIGeneration.h"

#include <iostream>
#include <fstream>


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

void Terrain::DeleteBiomes()
{
	for (int i = 0; i < biomes.size(); ++i)
	{
		delete biomes[i];
		biomes[i] = nullptr;
	}

	biomes.clear();
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

void Terrain::CalculateMaxBiomeHeight()
{
	maxBiomeHeight = 0.0f;

	for (int i = 0; i < biomes.size(); ++i)
	{
		if (maxBiomeHeight < biomes[i]->height)
		{
			maxBiomeHeight = biomes[i]->height;
		}
	}
}

void Terrain::SetNeighbours()
{
	for (int i = 0; i < totalkChunks; ++i)
	{
		chunks[i]->SetNeighbours();
	}
}

void Terrain::Save()
{
	//Terrain info size: numChunks, height, width, maxBiomeHeight
	uint tSize = sizeof(int) + sizeof(float) * 3;

	//Biome info size: numBiomes
	uint bSize = sizeof(int) * biomes.size();

	for (int i = 0; i < biomes.size(); ++i)
	{
		bSize += sizeof(char) * biomes[i]->name.length() + sizeof(int) * 3 + sizeof(float) * 3;
	}

	//Chunk info
	uint cSize = 0;

	for (int i = 0; i < chunks.size(); ++i)
	{
		cSize += sizeof(char) * chunks[i]->biome->name.length();
	}

	uint totalSize = tSize + bSize + cSize;

	char* buffer = new char[totalSize];
	char* cursor = buffer;

	//Saving Terrain info
	uint bytes = sizeof(numChunks);
	memcpy(cursor, &numChunks, bytes);
	cursor += bytes;

	bytes = sizeof(height);
	memcpy(cursor, &height, bytes);
	cursor += bytes;

	bytes = sizeof(width);
	memcpy(cursor, &width, bytes);
	cursor += bytes;

	bytes = sizeof(maxBiomeHeight);
	memcpy(cursor, &maxBiomeHeight, bytes);
	cursor += bytes;

	//Saving biome info
	uint biomesSize = biomes.size();
	bytes = sizeof(biomes.size());
	memcpy(cursor, &biomesSize, bytes);
	cursor += bytes;

	for (int i = 0; i < biomesSize; ++i)
	{
		bytes = sizeof(int);
		int biomeNameLength = biomes[i]->name.length();
		memcpy(cursor, &biomeNameLength, bytes);
		cursor += bytes;

		bytes = sizeof(char)*biomes[i]->name.length();
		memcpy(cursor, biomes[i]->name.c_str(), bytes);
		cursor += bytes;

		bytes = sizeof(biomes[i]->frequency);
		memcpy(cursor, &(biomes[i]->frequency), bytes);
		cursor += bytes;

		bytes = sizeof(biomes[i]->height);
		memcpy(cursor, &(biomes[i]->height), bytes);
		cursor += bytes;

		bytes = sizeof(biomes[i]->octaves);
		memcpy(cursor, &(biomes[i]->octaves), bytes);
		cursor += bytes;

		bytes = sizeof(biomes[i]->primitive);
		memcpy(cursor, &(biomes[i]->primitive), bytes);
		cursor += bytes;

		bytes = sizeof(biomes[i]->seed);
		memcpy(cursor, &(biomes[i]->seed), bytes);
		cursor += bytes;
	}
	
	for (int i = 0; i < chunks.size(); ++i)
	{
		bytes = sizeof(char)*chunks[i]->biome->name.length();
		memcpy(cursor, chunks[i]->biome->name.c_str(), bytes);
		cursor += bytes;
	}

	ofstream file;
	file.open("Text.trg", ios::out | ios::trunc | ios::binary);
	file.write(buffer, totalSize);

	file.close();
	delete[] buffer;
}

void Terrain::Load()
{
	myApp->m_ui->generationWindow->selectedBiome = nullptr;
	myApp->m_ui->generationWindow->selectedChunk = nullptr;
	DeleteBiomes();
	DeleteChunks();
	
	char* buffer;
	streampos size;
	ifstream file;

	file.open("Text.trg", ios::in | ios::binary | ios::ate);
	
	size = file.tellg();
	buffer = new char[54];
	file.seekg(0, ios::beg);
	file.read(buffer, 54);
	
	file.close();

	char* cursor = buffer;

	uint bytes = sizeof(numChunks);
	memcpy(&numChunks, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(height);
	memcpy(&height, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(width);
	memcpy(&width, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(maxBiomeHeight);
	memcpy(&maxBiomeHeight, cursor, bytes);
	cursor += bytes;

	//Loading biome info
	uint biomesSize = 0;
	bytes = sizeof(int);
	memcpy(&biomesSize, cursor, bytes);
	cursor += bytes;

	for (int i = 0; i < biomesSize; ++i)
	{
		int biomeNameSize = 0;
		bytes = sizeof(int);
		memcpy(&biomeNameSize, cursor, bytes);
		cursor += bytes;

		bytes = sizeof(char) * biomeNameSize;
		string biomeName(cursor, cursor + bytes);
		cursor += bytes;

		Biome* newBiome = new Biome(biomeName);
		//memcpy()
	}

	/*terrain->GenerateChunks(terrainChunks, terrainHeight, terrainWidth);
	terrain->SetNeighbours();
	terrain->parent->updateTFB = true;*/


}

