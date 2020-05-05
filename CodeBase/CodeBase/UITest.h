#ifndef __UITEST_H__
#define __UITEST_H__

#include "UIWindow.h"

#include <string>

class Terrain;
class Mesh;

class UITest :	public UIWindow
{
public:

	UITest(std::string name, bool active = true);
	~UITest();

	bool Draw();

public:

	GameObject* curr = nullptr;
	Terrain* terrain = nullptr;
	
};

#endif // !__UITEST_H__
