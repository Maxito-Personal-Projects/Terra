#include "ModuleWindow.h"
#include "Application.h"

bool ModuleWindow::Init()
{
	bool ret = true;

	LOG("Module Window Init!");

	float version =CODEBASE_VERSION;

	title += to_string(version);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
	
	if (!window)
	{
		ret = false;
		LOG("There was an error creating the window!");
	}

	return ret;
}

bool ModuleWindow::CleanUp()
{
	bool ret = true;

	SDL_DestroyWindow(window);
	window = nullptr;

	return ret;
}
