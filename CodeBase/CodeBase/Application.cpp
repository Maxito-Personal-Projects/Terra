#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"


Application::Application()
{
	//Create Modules here
	m_window = new ModuleWindow("Window");

	//Add them to module list
	modules.push_back(m_window);

}

Application::~Application()
{
	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end(); it_m++)
	{
		delete *it_m;
		*it_m = nullptr;		
	}
}

bool Application::Init()
{
	bool ret = true;
	string error = "";

	int SDL_error = SDL_Init(SDL_INIT_EVERYTHING);
	if (SDL_error != 0)
	{
		error = "Error Initializing SDL2" ;
		LOG("%s",error);

		ret = false;
	}
	else
	{
		LOG("SDL2 Initialized Correctly");
	}

	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret=(*it_m)->Init();

		if (!ret)
		{
			error = "Error Initializing Module" + (*it_m)->name;
			cout << error << endl;
		}
	}

	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret = (*it_m)->Start();

		if (!ret)
		{
			error = "Error Starting Module" + (*it_m)->name;
			cout << error << endl;
		}
	}

	return ret;
}

bool Application::Update()
{
	bool ret = true;
	string error = "";

	//PreUpdate ---------------------------------
	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret = (*it_m)->PreUpdate();

		if (!ret)
		{
			error = "Error PreUpdating Module" + (*it_m)->name;
			cout << error << endl;
		}
	}
	
	//Update ------------------------------------
	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret = (*it_m)->Update();

		if (!ret)
		{
			error = "Error Updating Module" + (*it_m)->name;
			cout << error << endl;
		}
	}
	
	//PostUpdate --------------------------------
	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret = (*it_m)->PosUpdate();

		if (!ret)
		{
			error = "Error PostUpdating Module" + (*it_m)->name;
			cout << error << endl;
		}
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	string error = "";

	//PostUpdate --------------------------------
	for (list<Module*>::iterator it_m = modules.begin(); it_m != modules.end() && ret; it_m++)
	{
		ret = (*it_m)->CleanUp();

		if (!ret)
		{
			error = "Error Cleaning Module" + (*it_m)->name;
			cout << error << endl;
		}
	}

	SDL_Quit();

	return ret;
}
