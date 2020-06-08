#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleCamera.h"
#include "ModuleUI.h"
#include "UITest.h"
#include "GameObject.h"
#include "Texture.h"
#include "Mesh.h"
#include "Terrain.h"

UITest::UITest(std::string name, bool active):UIWindow(name,active)
{
}


UITest::~UITest()
{
	curr = nullptr;
}

bool UITest::Draw()
{
	bool ret = true;

	if (!terrain && myApp->m_render->firstGO)
	{
		terrain = myApp->m_render->firstGO->terrain;
	}

	ImGui::Begin(name.c_str(), &active);

	// Render setting
	ImGui::PushFont(myApp->m_ui->arial);
	ImGui::Text("Render Settings:");
	ImGui::Text("Light Direction:");
	ImGui::SameLine();
	ImGui::PushID("Light Dir");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat3("", myApp->m_render->lightDirection, 0.01f, -1.0f, 1.0f, "%.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("-------------------------");
	
	// Cam setting
	ImGui::Text("Cam Settings:");
	ImGui::Text("Cam Boost:");
	ImGui::SameLine();
	ImGui::PushID("Speed Boost");
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("", &myApp->m_camera->camBoost, 0.1f,NULL,"%0.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("-------------------------");

	ImGui::Text("Result:");

	ImGui::Image((void*)(intptr_t)myApp->m_render->exportTexture, ImVec2(200, 200));

	ImGui::PopFont();


	ImGui::End();

	return ret;
}
