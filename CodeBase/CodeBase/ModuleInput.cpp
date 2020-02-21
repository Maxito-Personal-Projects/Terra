#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"


ModuleInput::ModuleInput(string _name, bool _active) : Module(_name, _active)
{
	keyboard = new KEY_STATE[MAX_KEYS];
}

ModuleInput::~ModuleInput()
{
}

bool ModuleInput::Init()
{
	LOG("Module Input Init!");

	int sdlError = SDL_InitSubSystem(SDL_INIT_EVENTS);
	if (sdlError != 0)
	{
		LOG("Error initializing SDL events");
		return false;
	}

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		keyboard[i] = IDLE;
	}

	return true;
}

bool ModuleInput::PreUpdate()
{
	bool ret = true;

	//Needed to get any type of event
	SDL_PumpEvents();

	//Getting the state of every key 
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		//Check if the key is pressed(1) or not(0)
		if (keyState[i] == 1)
		{
			if (keyboard[i] == IDLE)
			{
				keyboard[i] = DOWN;
			}
			else	//Already pressed
			{
				keyboard[i] = REPEAT;
			}
		}
		else
		{
			if (keyboard[i] == DOWN || keyboard[i] == REPEAT)
			{
				keyboard[i] = UP;
			}
			else
			{
				keyboard[i] = IDLE;
			}
		}
	}

	SDL_Event myEvent;

	//Getting all events
	while (SDL_PollEvent(&myEvent))
	{
		switch (myEvent.type)
		{
		case SDL_QUIT:
			myApp->exit = true;
			break;
		
		//Managing window events
		case SDL_WINDOWEVENT:
			switch (myEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				myApp->m_render->ResizeWindow();
				LOG("Window size has changed!")
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				//TODO(Max) Call resize viewport function
				LOG("Window size changed!")
				break;
			default:
				break;
			}
		
		default:
			break;
		}
	}

	//TODO(max) SDL_Events!!!

	if (GetKey(SDL_SCANCODE_ESCAPE) == DOWN)
	{
		myApp->exit = true;
	}

	return ret;
}

bool ModuleInput::CleanUp()
{
	delete[] keyboard;

	return true;
}

KEY_STATE ModuleInput::GetKey(int keyCode)
{
	return keyboard[keyCode];
}
