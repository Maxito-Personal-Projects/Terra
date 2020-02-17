#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"



ModuleRenderer::ModuleRenderer(string _name, bool _active) :Module(_name, _active)
{
}

ModuleRenderer::~ModuleRenderer()
{
}

bool ModuleRenderer::Init()
{
	bool ret = true;

	LOG("Module Render Init!");

	context = SDL_GL_CreateContext(myApp->m_window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_GL_SetSwapInterval(1);
	
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		LOG("Error initializing glew with error: %s", glewGetErrorString(error));
		ret = false;
	}

	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	return ret;
}



bool ModuleRenderer::PosUpdate()
{
	bool ret = true;

	//Clear the buffers before drawing!
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	
	
	-------- Draw Stuff --------
	
	
	*/

	//Test!
	if (myApp->m_input->GetKey(SDL_SCANCODE_SPACE)==DOWN)
	{
		glClearColor(0, 1, 1, 1);
	}

	//Update the window with OpenGL rendering
	SDL_GL_SwapWindow(myApp->m_window->window);

	return ret;
}

bool ModuleRenderer::CleanUp()
{
	SDL_GL_DeleteContext(context);

	return true;
}
