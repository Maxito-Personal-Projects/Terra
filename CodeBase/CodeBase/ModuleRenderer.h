#ifndef __MODULERENDERER_H__
#define __MODULERENDERER_H__

#include "Module.h"

class GameObject;

class ModuleRenderer : public Module
{
public:

	ModuleRenderer(string _name, bool _active = true);
	~ModuleRenderer();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start();

	//PreUpdate Module
	bool PreUpdate();

	//Update Module
	bool Update() { return true; }

	//PostUpdate Module
	bool PosUpdate();

	//Clean Module
	bool CleanUp();

	//Resize window & viewport
	void ResizeWindow();

public:

	SDL_GLContext context;
	GameObject* firstGO = nullptr;

	bool wired = false;

};

#endif // !__MODULERENDERER_H__
