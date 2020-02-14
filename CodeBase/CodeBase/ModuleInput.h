#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"

#define MAX_KEYS 100

enum KEY_STATE
{
	IDLE,
	DOWN,
	REPEAT,
	UP
};

class ModuleInput :	public Module
{
public:

	ModuleInput(string _name, bool _active = true);
	
	~ModuleInput();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start() { return true; }

	//PreUpdate Module
	bool PreUpdate();

	//Update Module
	bool Update() { return true; }

	//PreUpdate Module
	bool PosUpdate() { return true; }

	//Clean Module
	bool CleanUp();

	KEY_STATE GetKey(int keyCode);

public:

	KEY_STATE* keyboard;

};

#endif // !__MODULEINPUT_H__
