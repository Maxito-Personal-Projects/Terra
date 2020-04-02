#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleUI.h"
#include "UITest.h"



UITest::UITest(std::string name, bool active):UIWindow(name,active)
{
}


UITest::~UITest()
{
}

bool UITest::Draw()
{
	bool ret = true;

	ImGui::Begin(name.c_str(), &active);
	ImGui::Text("Cam Boost:");
	ImGui::SameLine();
	ImGui::PushID("Speed Boost");
	ImGui::InputFloat("", &myApp->m_camera->camBoost, 0.1f,NULL,"%0.1f");
	ImGui::PopID();
	ImGui::End();

	return ret;
}
