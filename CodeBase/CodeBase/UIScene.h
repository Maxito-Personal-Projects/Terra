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

	float2 imagePosition = { 0.0f,0.0f };

};

#endif // !__UISCENE_H__
