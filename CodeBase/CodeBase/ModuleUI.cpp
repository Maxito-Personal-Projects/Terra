#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "Texture.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Terrain.h"

#include "UIWindow.h"
#include "UITest.h"
#include "UIExport.h"
#include "UIGeneration.h"
#include "UIScene.h"
#include "UIChunk.h"


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

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(myApp->m_window->window, myApp->m_render->context);

	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Windows 
	testWindow = new UITest("Settings");
	exportWindow = new UIExport("Export",false);
	generationWindow = new UIGeneration("Generate");
	sceneWindow = new UIScene("Scene");
	chunkWindow = new UIChunk("Chunk Properties");

	windows.push_back(testWindow);
	windows.push_back(exportWindow);
	windows.push_back(generationWindow);
	windows.push_back(sceneWindow);
	windows.push_back(chunkWindow);

	//Fonts 
	defaultFont = io.Fonts->AddFontDefault();
	arial = io.Fonts->AddFontFromFileTTF("Fonts/Arial.ttf", 16.0f);
	boldKei = io.Fonts->AddFontFromFileTTF("Fonts/BoldKei.ttf", 15.0f);
	boldTestament = io.Fonts->AddFontFromFileTTF("Fonts/BoldTestament.ttf", 20.0f);
	montserrat = io.Fonts->AddFontFromFileTTF("Fonts/Montserrat-Regular.ttf", 15.0f);
	montserratBold = io.Fonts->AddFontFromFileTTF("Fonts/Montserrat-Bold.ttf", 22.0f);

	return ret;
}

bool ModuleUI::Start()
{
	bool ret = true;

	dragImage = myApp->fileSystem->LoadImagePNG("Images/Error.png");
	textTest = myApp->fileSystem->LoadImagePNG("Images/image_82566.png");

	return ret;
}

bool ModuleUI::PreUpdate()
{
	bool ret = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(myApp->m_window->window);
	ImGui::NewFrame();

	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)myApp->m_window->width, (float)myApp->m_window->height });
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("D", 0, window_flags);
	ImGui::PopStyleVar(3);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Export Terrain"))
			{
				exportWindow->active = true;
				exportWindow->exportMessage = "";
			}
			if (ImGui::MenuItem("Exit", "ESC"))
			{
				myApp->exit = true;
			}
			if (ImGui::MenuItem("Save"))
			{
				myApp->m_render->firstGO->terrain->Save();
			}
			if (ImGui::MenuItem("Load"))
			{
				myApp->m_render->firstGO->isLoading=true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Settings"))
			{
				testWindow->active=true;
			}
			if (ImGui::MenuItem("Generation"))
			{
				generationWindow->active = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Tutorial"))
			{
				ShellExecuteA(NULL,"open","https://github.com/Maxito-Personal-Projects/CodeBase/wiki/Tutorial", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();


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
