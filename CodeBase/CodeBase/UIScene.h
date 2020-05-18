#ifndef __UISCENE_H__
#define __UISCENE_H__

#include "UIWindow.h"

class UIScene : public UIWindow
{
public:

	UIScene(std::string name, bool active = true);
	~UIScene();

	bool Draw();

public:

	int width;
	int height;

	bool focused = false;
};

#endif // !__UISCENE_H__
