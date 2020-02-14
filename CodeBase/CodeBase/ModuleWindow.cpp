#include "ModuleWindow.h"
#include "Application.h"

bool ModuleWindow::Init()
{
	bool ret = true;

	LOG("Module Window Init!");

	int sdlError = SDL_Init(SDL_INIT_VIDEO);
	if (sdlError != 0)
	{
		LOG("Error initializing SDL video");
		return false;
	}

	char test[24] = "";
	sprintf(test, "%.1f", CODEBASE_VERSION);
	title += test;

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

	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	return ret;
}
