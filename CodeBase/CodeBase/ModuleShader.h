#ifndef __MODULESHADER_H__
#define __MODULESHADER_H__

#include "Module.h"
class ModuleShader : public Module
{
public:

	ModuleShader(string _name, bool _active=true);
	~ModuleShader();

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

};

#endif // !__MODULESHADER_H__
