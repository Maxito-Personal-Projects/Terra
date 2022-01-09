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
	void DeleteBiomes();

	Chunk* GetChunkFromCoords(int x, int y, int &num);
	void CalculateMaxBiomeHeight();
	void SetNeighbours();

	void Save();
	void Load();

public:

	GameObject* parent = nullptr;
	std::vector<Chunk*> chunks;
	std::vector<Biome*> biomes;
	
	int totalkChunks = 0;
	int numChunks = 1;

	float width = 64.0f;
	float height = 64.0f;

	float maxBiomeHeight = 0.0f;

};

#endif // !__TERRAIN_H__
