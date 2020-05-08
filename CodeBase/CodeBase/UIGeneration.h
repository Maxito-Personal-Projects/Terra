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
	string fileName;

	Texture* heightmap = nullptr;

};

#endif // !__UIGENERATION_H__