#include <iostream>
#include "Application.h"

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

int main()
{
	MainState state = MAIN_INIT;

	while (state!=MAIN_EXIT)
	{
		switch (state)
		{

		case MAIN_INIT:
			
			cout << "Creating MyApp ------------------------------" << endl;
			
			myApp = new Application();

			if (myApp)
			{
				cout << "Application Created Succesfully" << endl;
			}
			
			state = MAIN_START;

			break;

		case MAIN_START:

			cout << "Starting MyApp ------------------------------" << endl;
			
			myApp = new Application();
			
			state = MAIN_UPDATE;

			break;

		case MAIN_UPDATE:

			cout << "Updating MyApp ------------------------------" << endl;
			
			myApp = new Application();
			
			state = MAIN_CLEAN;

			break;

		case MAIN_CLEAN:

			cout << "Cleaning MyApp before closing ------------------------------" << endl;

			state = MAIN_EXIT;

			break;
		}

	}

	delete myApp;
	myApp = nullptr;

	cout << "Closing MyApp ------------------------------" << endl;

	cout << endl;
	cout << "C YA!" << endl;


	system("pause");
	return 0;
}