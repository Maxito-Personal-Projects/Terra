#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Module.h"

class Camera;

class ModuleCamera : public Module
{
public:

	ModuleCamera(string _name, bool _active = true);
	~ModuleCamera();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start() { return true; }

	//PreUpdate Module
	bool PreUpdate() { return true; }

	//Update Module
	bool Update();

	//PreUpdate Module
	bool PosUpdate() { return true; };

	//Clean Module
	bool CleanUp();

public:

	Camera* mainCamera = nullptr;
};

#endif // !__MODULECAMERA_H__
