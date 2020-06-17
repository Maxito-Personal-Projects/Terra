#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "UIScene.h"
#include "FileSystem.h"
#include "UIGeneration.h"


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

	for (int i = 0; i < 5; ++i)
	{
		mouse[i] = IDLE;
	}

	return true;
}

bool ModuleInput::PreUpdate()
{
	bool ret = true;
	ImGuiIO& io = ImGui::GetIO();

	//Needed to get any type of event
	SDL_PumpEvents();

	//Getting the state of every key 
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	Uint8 mouseState = SDL_GetMouseState(&mouseX, &mouseY);



	//Keyboard inputs
	for (int i = 0; i < MAX_KEYS && myApp->m_ui->sceneWindow->focused; ++i)
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

	//Mouse inputs

	for (int i = 0; i < 5 && myApp->m_ui->sceneWindow->focused; ++i)
	{
		//Check if the key is pressed(1) or not(0)
		if (mouseState & SDL_BUTTON(i))
		{
			if (mouse[i] == IDLE)
			{
				mouse[i] = DOWN;
			}
			else	//Already pressed
			{
				mouse[i] = REPEAT;
			}
		}
		else
		{
			if ((mouse[i] == DOWN || mouse[i] == REPEAT) )
			{
				mouse[i] = UP;
			}
			else
			{
				mouse[i] = IDLE;
			}
		}
	}

	SDL_Event myEvent;

	//Reset motion every frame
	motionX = motionY = 0;

	//Getting all events
	while (SDL_PollEvent(&myEvent))
	{
		ImGui_ImplSDL2_ProcessEvent(&myEvent);
		std::string path;

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
				SDL_GetWindowSize(myApp->m_window->window, &myApp->m_window->width, &myApp->m_window->height);
				LOG("Window size has changed!");
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				//TODO(Max) Call resize viewport function
				LOG("Window size changed!");
				break;
			default:
				break;
			}
		case SDL_MOUSEMOTION:
			//new mouse pos because of moving
			mouseX = myEvent.motion.x;
			mouseY = myEvent.motion.y;

			mouseSceneX = mouseX - myApp->m_ui->sceneWindow->imagePosition.x;
			mouseSceneY = mouseY - myApp->m_ui->sceneWindow->imagePosition.y;

			//relative position to detect movement
			motionX = myEvent.motion.xrel;
			motionY = myEvent.motion.yrel;

			break;

		case SDL_DROPFILE:

			SDL_RaiseWindow(myApp->m_window->window);
			myApp->m_ui->generationWindow->askForDrag = true;
			myApp->m_ui->generationWindow->pathToDrag = myEvent.drop.file;
		
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

KEY_STATE ModuleInput::GetMouseButton(int buttonCode)
{
	return mouse[buttonCode];
}
