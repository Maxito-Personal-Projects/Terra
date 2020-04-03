#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"

#include "UIWindow.h"
#include "UITest.h"


ModuleUI::ModuleUI(string _name, bool _active) : Module(_name,_active)
{
}


ModuleUI::~ModuleUI()
{
	for (std::list<UIWindow*>::iterator w_it = windows.begin(); w_it != windows.end(); w_it++)
	{
		delete (*w_it);
		(*w_it) = nullptr;
	}
}

bool ModuleUI::Init()
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(myApp->m_window->window, myApp->m_render->context);

	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	testWindow = new UITest("Test Window");

	windows.push_back(testWindow);

	return ret;
}

bool ModuleUI::PreUpdate()
{
	bool ret = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(myApp->m_window->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "ESC"))
			{
				myApp->exit = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				testWindow->active=true;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	return ret;
}

bool ModuleUI::PosUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::CleanUp()
{
	bool ret = true;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

void ModuleUI::DrawUI()
{
	for (std::list<UIWindow*>::iterator w_it = windows.begin(); w_it != windows.end(); w_it++)
	{
		if ((*w_it)->active)
		{
			(*w_it)->Draw();
		}
	}
}
