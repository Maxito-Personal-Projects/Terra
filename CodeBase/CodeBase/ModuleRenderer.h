#ifndef __MODULERENDERER_H__
#define __MODULERENDERER_H__

#include "Module.h"

class ModuleRenderer : public Module
{
public:

	ModuleRenderer(string _name, bool _active = true);
	~ModuleRenderer();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start() { return true; }

	//PreUpdate Module
	bool PreUpdate() { return true; }

	//Update Module
	bool Update() { return true; }

	//PostUpdate Module
	bool PosUpdate();

	//Clean Module
	bool CleanUp();

public:

	SDL_GLContext context;
};

#endif // !__MODULERENDERER_H__
