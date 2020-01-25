#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>

using namespace std;

class Module
{
public:

	Module();
	~Module();

	//Init Module, Used to create Stuff
	virtual bool Init() { return true; }
	
	//Starting Module, Used to init Stuff
	virtual bool Start() { return true; }

	//PreUpdate Module
	virtual bool PreUpdate() { return true; }

	//Update Module
	virtual bool Update() { return true; }

	//PreUpdate Module
	virtual bool PosUpdate() { return true; }

	//Clean Module
	virtual bool CleanUp() { return true; }

public:

	string name = "";

	bool active = false;

};

#endif //__MODULE_H__