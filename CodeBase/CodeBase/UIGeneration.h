#ifndef __UIGENERATION_H__
#define __UIGENERATION_H__

#include "UIWindow.h"

class Texture;
class Terrain;
class Chunk;
class Biome;

class UIGeneration : public UIWindow
{
public:

	UIGeneration(std::string name, bool active = true);
	~UIGeneration();

	void LoadHeightMap(string path);
	void SetDefaultColors();

	bool Draw();

private:

	char* currPrimitive = nullptr;
	char* currFunction = nullptr;
	char* currNumLayer = nullptr;
	char* currTexture[6];

	float terrainHeight = 0;
	float terrainWidth = 0;
	int terrainChunks = 0;


	bool heightWindow = false;

	string imageName;
	string biomeName;

	Terrain* terrain = nullptr;

public:

	Texture* heightmap = nullptr;

	float layerRanges[7];
	float3 layerColors[8];
	int layerTypes[6];

	int numLayers = 1;

	Chunk* selectedChunk = nullptr;
	Biome* selectedBiome = nullptr;
	string currBiome = "";

	string pathToDrag = "";

	bool askForDrag = false;
	bool canDrag = false;
	bool showBiome = false;

};

#endif // !__UIGENERATION_H__