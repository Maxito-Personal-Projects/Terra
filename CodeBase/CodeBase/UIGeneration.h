#ifndef __UIGENERATION_H__
#define __UIGENERATION_H__

#include "UIWindow.h"

class Texture;
class Terrain;

class UIGeneration : public UIWindow
{
public:

	UIGeneration(std::string name, bool active = true);
	~UIGeneration();

	bool Draw();

private:

	char* currPrimitive = nullptr;
	char* currFunction = nullptr;
	char* currNumLayer = nullptr;

	float terrainHeight = 0;
	float terrainWidth = 0;
	int terrainChunks = 0;

	int numLayers = 1;

	string fileName;

	Terrain* terrain = nullptr;

public:

	Texture* heightmap = nullptr;

};

#endif // !__UIGENERATION_H__