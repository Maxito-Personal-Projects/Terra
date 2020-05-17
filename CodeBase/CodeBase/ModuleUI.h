#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <list>

class UIWindow;
class UITest;
class UIExport;
class UIGeneration;
class UIScene;

class Texture;

class ModuleUI : public Module
{
public:

	ModuleUI(string _name, bool _active = true);
	~ModuleUI();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start();

	//PreUpdate Module
	bool PreUpdate();

	//Update Module
	bool Update() { return true; }

	//PreUpdate Module
	bool PosUpdate();

	//Clean Module
	bool CleanUp();

	void DrawUI();

private:

	std::list<UIWindow*> windows;

	UITest* testWindow = nullptr;
	UIExport* exportWindow = nullptr;
	UIGeneration* generationWindow = nullptr;

public:

	UIScene* sceneWindow = nullptr;

	Texture* errorImage = nullptr;
	Texture* textTest = nullptr;

	ImFont* defaultFont = nullptr;
	ImFont* arial = nullptr;
	ImFont* boldKei = nullptr;
	ImFont* boldTestament = nullptr;
	ImFont* montserrat = nullptr;
	ImFont* montserratBold = nullptr;


};

#endif // !__MODULEUI_H__
