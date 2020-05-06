#ifndef __UIGENERATION_H__
#define __UIGENERATION_H__

#include "UIWindow.h"

class UIGeneration : public UIWindow
{
public:

	UIGeneration(std::string name, bool active = true);
	~UIGeneration();

	bool Draw();

};

#endif // !__UIGENERATION_H__