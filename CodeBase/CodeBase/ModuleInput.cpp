#include "Application.h"
#include "ModuleInput.h"


ModuleInput::ModuleInput(string _name, bool _active) : Module(_name, _active)
{
	keyboard = new KEY_STATE[MAX_KEYS];
}

ModuleInput::~ModuleInput()
{
}

bool ModuleInput::Init()
{
	LOG("Module Window Init!");

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
	LOG("Input PreUpdate!");
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

	if (GetKey(SDL_SCANCODE_ESCAPE) == DOWN)
	{
		ret = false;
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
