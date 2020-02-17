#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include <iostream>
#include "Globals.h"
#include "Glew/include/glew.h"

#include "SDL/include/SDL.h"
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer;


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
	ModuleInput* m_input = nullptr;
	ModuleRenderer* m_render = nullptr;

	bool exit = false;

};

extern Application* myApp;

#endif // !__APPLICATION_H__
