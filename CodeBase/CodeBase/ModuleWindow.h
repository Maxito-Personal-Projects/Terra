#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include <iomanip> //for set precision
#include "Module.h"

class SDL_Window;

class ModuleWindow: public Module
{
public: 
	ModuleWindow(string _name, bool _active = true) :Module(_name, _active) {}
	~ModuleWindow(){}

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start() { return true; }

	//PreUpdate Module
	bool PreUpdate() { return true; }

	//Update Module
	bool Update() { return true; }

	//PreUpdate Module
	bool PosUpdate() { return true; }

	//Clean Module
	bool CleanUp();

public:

	SDL_Window* window = nullptr;

	int width = 1200;
	int height = 1000;

	string title = "CodeBase v";

};

#endif // !__MODULEWINDOW_H__
