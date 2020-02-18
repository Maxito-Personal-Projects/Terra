#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

class ModuleUI : public Module
{
public:

	ModuleUI(string _name, bool _active = true);
	~ModuleUI();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start() { return true; }

	//PreUpdate Module
	bool PreUpdate();

	//Update Module
	bool Update() { return true; }

	//PreUpdate Module
	bool PosUpdate();

	//Clean Module
	bool CleanUp();
};

#endif // !__MODULEUI_H__
