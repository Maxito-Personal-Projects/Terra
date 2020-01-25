#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include <iostream>

class Module;

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



};

#endif // !__APPLICATION_H__
