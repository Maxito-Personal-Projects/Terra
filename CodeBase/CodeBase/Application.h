#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include <iostream>
#include "Globals.h"

#include "SDL/include/SDL.h"

class Module;
class ModuleWindow;

using namespace std;

class Application
{
public:

	Application();
	~Application();

	//Initializing all variables and Modules
	bool Init();

	//PreUpdating, Updating and PostUpdating all Modules
	bool Update();

	//Cleaning all modules
	bool CleanUp();

private:

	list<Module*> modules;

public:

	ModuleWindow* m_window = nullptr;



};

#endif // !__APPLICATION_H__
