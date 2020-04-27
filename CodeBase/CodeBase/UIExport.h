#ifndef __UIEXPORTER_H__
#define __UIEXPORTER_H__

#include "UIWindow.h"

class GameObject;
class Terrain;
class Mesh;

enum ExportFormat;

class UIExport : public UIWindow
{
public:

	UIExport(std::string name, bool active = true);
	~UIExport();

	bool Draw();

private:

	bool dae = false;
	bool obj_m = false;
	bool obj = false;
	bool fbx = false;

	ExportFormat format;

	GameObject* curr = nullptr;
	Terrain* terrain = nullptr;
	Mesh* mesh = nullptr;

	std::string fileName = "";

public:

	std::string exportMessage = "";

};

#endif // !__UIEXPORTER_H__