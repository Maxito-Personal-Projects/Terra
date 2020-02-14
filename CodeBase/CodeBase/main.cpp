#include <iostream>
#include "Application.h"

#include "SDL/include/SDL.h"
#pragma comment(lib,"SDL/libx86/SDL2.lib")
#pragma comment(lib,"SDL/libx86/SDL2main.lib")

using namespace std;

enum MainState 
{
	MAIN_INIT,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_CLEAN,
	MAIN_EXIT
};

Application* myApp = nullptr;

int main(int argc, char** argv)
{
	MainState state = MAIN_INIT;

	while (state!=MAIN_EXIT)
	{
		switch (state)
		{

		case MAIN_INIT:
						
			myApp = new Application();

			if (myApp)
			{
				LOG("Application Created Succesfully");
			}
			
			state = MAIN_START;

			break;

		case MAIN_START:

			LOG("Starting MyApp ------------------------------");
			
			if (myApp->Init())
			{
				state = MAIN_UPDATE;
			}
			else
			{
				LOG("FATAL ERROR: Start Went Wrong")
				state = MAIN_CLEAN;
			}

			break;

		case MAIN_UPDATE:

			LOG("Updating MyApp ------------------------------");

			if (!myApp->Update())
			{
				LOG("FATAL ERROR: Start Went Wrong")
					state = MAIN_CLEAN;
			}
			
			break;

		case MAIN_CLEAN:

			LOG("Cleaning MyApp before closing ------------------------------");

			if (!myApp->CleanUp())
			{
				LOG("FATAL ERROR: CleanUp Went Wrong")
			}

			state = MAIN_EXIT;

			break;
		}

	}

	delete myApp;
	myApp = nullptr;

	LOG("Closing MyApp ------------------------------");

	LOG("C YA!");
	
	return 0;
}