#ifndef __UIWINDOW_H__
#define __UIWINDOW_H__

#include <string>

class UIWindow
{
public:

	UIWindow(std::string _name, bool _active);
	~UIWindow();

	virtual bool Draw() { return true; }

public:
	
	bool active = true;
	std::string name = "";

};

#endif // !__UIWINDOW_H__
