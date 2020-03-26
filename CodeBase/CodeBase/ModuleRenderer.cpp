#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleUI.h"

#include "GameObject.h"



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

	//Telling OpenGL the size of the rendering window
	glViewport(0, 0, myApp->m_window->width, myApp->m_window->height);
	//TODO(max): A function that change the viewport when the window is modified (need sdl events!)

	glClearColor(1, 0.55f, 0.48f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);;

	return ret;
}

bool ModuleRenderer::Start()
{
	bool ret = true;

	firstGO = new GameObject();

	return ret;
}

bool ModuleRenderer::PreUpdate()
{
	bool ret = true;



	return ret;
}



bool ModuleRenderer::PosUpdate()
{
	bool ret = true;

	//Clear the buffers before drawing!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw all game objects (now one)
	firstGO->Draw();

	//Unbinding all buffers
	glBindVertexArray(0);

	//Test!
	if (myApp->m_input->GetKey(SDL_SCANCODE_SPACE) == DOWN)
	{
		glClearColor(0, 1, 1, 1);
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Update the window with OpenGL rendering
	SDL_GL_SwapWindow(myApp->m_window->window);

	return ret;
}

bool ModuleRenderer::CleanUp()
{
	SDL_GL_DeleteContext(context);

	delete firstGO;
	firstGO = nullptr;

	return true;
}

void ModuleRenderer::ResizeWindow()
{
	//Change windo Size
	SDL_GetWindowSize(myApp->m_window->window, &myApp->m_window->width, &myApp->m_window->height);

	//Change Viewport Size
	glViewport(0, 0, myApp->m_window->width, myApp->m_window->height);

}
