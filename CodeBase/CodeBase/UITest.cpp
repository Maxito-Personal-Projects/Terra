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

	ImGui::PushFont(myApp->m_ui->arial);

	ImGui::Text("Global Settings:");

	//Window DrawList
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	//Title Underline 
	ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
	ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
	drawList->AddLine(underLinePosL, underLinePosR, ImColor(1.0f, 1.0f, 1.0f, 1.0f));
	
	ImGui::Dummy(ImVec2(0.0, 5.0));

	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0.0f, -2.0f));
	ImGui::Text("Light Direction:");
	ImGui::Dummy(ImVec2(0.0f, 1.0f));
	ImGui::Text("Cam Boost:");
	ImGui::Dummy(ImVec2(0.0f, 1.0f));
	ImGui::Text("RotationSpeed:");
	ImGui::Dummy(ImVec2(0.0f, 1.0f));
	ImGui::Text("Reset Camera");

	ImGui::EndGroup();

	ImGui::SameLine();

	// Render setting
	ImGui::BeginGroup();
	
	ImGui::PushID("Light Dir");
	ImGui::PushItemWidth(200.0f);
	ImGui::DragFloat3("", myApp->m_render->lightDirection, 0.01f, -1.0f, 1.0f, "%.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	
	ImGui::PushID("Speed Boost");
	ImGui::PushItemWidth(200.0f);
	ImGui::InputFloat("", &myApp->m_camera->camBoost, 0.1f,NULL,"%0.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	
	ImGui::PushID("SpeedRotationBoost");
	ImGui::PushItemWidth(200.0f);
	ImGui::InputFloat("", &myApp->m_camera->rotationSpeed, 0.1f, NULL, "%0.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	
	ImGui::PushID("ResetPos");
	if (ImGui::Button("Reset"))
	{
		myApp->m_camera->ResetCameraPosition();
	}
	ImGui::PopID();

	ImGui::EndGroup();

	ImGui::PopFont();

	ImGui::End();

	return ret;
}
