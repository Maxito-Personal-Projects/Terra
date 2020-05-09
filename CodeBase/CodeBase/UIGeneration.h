#ifndef __UIGENERATION_H__
#define __UIGENERATION_H__

#include "UIWindow.h"

class Texture;


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

	int numLayers = 1;

	string fileName;

	Texture* heightmap = nullptr;

};

#endif // !__UIGENERATION_H__