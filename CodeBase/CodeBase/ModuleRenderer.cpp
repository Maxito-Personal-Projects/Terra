#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleUI.h"

#include "GameObject.h"

#include "UIScene.h"


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

	int MaxPatchVertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
	LOG("Max supported patch vertices %d\n", MaxPatchVertices);


	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glClearColor(1, 0.55f, 0.48f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return ret;
}

bool ModuleRenderer::Start()
{
	bool ret = true;

	firstGO = new GameObject();

	glGenTextures(1, &exportTexture);
	glBindTexture(GL_TEXTURE_2D, exportTexture);

	glTexStorage2D(GL_TEXTURE_2D,1, GL_RGBA8,100,100);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindImageTexture(0, exportTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	glBindTexture(GL_TEXTURE_2D, 0);

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

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);
	
	if (myApp->m_input->GetKey(SDL_SCANCODE_F) == DOWN) 
	{
		wired = !wired;
		if (wired)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	//Clear the buffers before drawing!
	glClear(GL_COLOR_BUFFER_BIT);

	// Rendering
	myApp->m_ui->DrawUI();
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

void ModuleRenderer::ResizeWindow(int x, int y)
{
	//Change Viewport Size
	glViewport(0, 0, x, y);

	GenerateFrameBuffer(x,y);

}

void ModuleRenderer::GenerateFrameBuffer(int x, int y)
{
	if (frameBuffer == 0)
	{
		glGenFramebuffers(1, &frameBuffer);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	if (fbTexture != 0)
	{
		glDeleteTextures(1, &fbTexture);
	}
	
	if (renderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &renderBuffer);
	}

	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x,y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x, y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Error Generating the framebuffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
