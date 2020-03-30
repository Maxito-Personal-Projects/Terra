#ifndef __UITEST_H__
#define __UITEST_H__

#include "UIWindow.h"

#include <string>

class UITest :	public UIWindow
{
public:

	UITest(std::string name, bool active = true);
	~UITest();

	bool Draw();



};

#endif // !__UITEST_H__
