#include "ModuleWindow.h"
#include "Application.h"

bool ModuleWindow::Init()
{
	bool ret = true;

	LOG("Module Window Init!");

	//Initializing SDL video subsytem
	int sdlError = SDL_Init(SDL_INIT_VIDEO);
	if (sdlError != 0)
	{
		LOG("Error initializing SDL video");
		return false;
	}

	//Showing App version
	char test[24] = "";
	sprintf(test, "%.1f", CODEBASE_VERSION);
	title += test;
	title += " - By: MaxitoSama";

	//Use OpenGL 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Creating SDL_OpenGL window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	if (!window)
	{
		ret = false;
		LOG("There was an error creating the window!");
	}

	icon = SDL_LoadBMP("Images/icon.bmp");
	SDL_SetWindowIcon(window, icon);

	return ret;
}

bool ModuleWindow::CleanUp()
{
	bool ret = true;

	//Destroying SDL window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	return ret;
}
