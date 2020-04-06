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
	float divisions = 100.0f;
	float maxHeight = 10.0f;
	float seed = 1.0f;
	float delta = 0.01f;
	float lightDirection[3] = { 1.0f,0.5f,1.0f };

};

#endif // !__MODULERENDERER_H__
