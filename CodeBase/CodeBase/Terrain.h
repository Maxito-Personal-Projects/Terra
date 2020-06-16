#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <vector>

class GameObject;
class Chunk;
class Biome;

class Terrain
{
public:

	Terrain(GameObject* _parent, int nChunks);
	~Terrain();

	void DrawChunks(bool updateTFB);
	void DrawSelectionChunks();
	void GenerateChunks(int nChunks, float height, float width);
	void DeleteChunks();
	void AddBiome(string name);
	void DeleteBiome();

	Chunk* GetChunkFromCoords(int x, int y, int &num);
	void SetNeighbours();

public:

	GameObject* parent = nullptr;
	std::vector<Chunk*> chunks;
	std::vector<Biome*> biomes;
	
	float maxHeight = 20.0f;
	float seed = 1.0f;
	float delta = 0.01f;
	float frequency = 1.0f;
	int octaves = 8;
	
	int totalkChunks = 0;
	int numChunks = 1;

	float width = 64.0f;
	float height = 64.0f;

	int primitive = 0;

};

#endif // !__TERRAIN_H__
