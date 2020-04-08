#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleCamera.h"
#include "ModuleUI.h"
#include "UITest.h"
#include "Texture.h"



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

	ImGui::Text("Mesh Divisions:");
	ImGui::SameLine();
	ImGui::PushID("Divisions");
	ImGui::DragFloat("", &myApp->m_render->divisions, 1.0f,1.0f);
	ImGui::PopID();

	ImGui::Text("Mesh Height:");
	ImGui::SameLine();
	ImGui::PushID("Height");
	ImGui::DragFloat("", &myApp->m_render->maxHeight, 1.0f,0.0f);
	ImGui::PopID();
	
	ImGui::Text("Mesh Seed:");
	ImGui::SameLine();
	ImGui::PushID("Seed");
	ImGui::DragFloat("", &myApp->m_render->seed, 0.01f);
	ImGui::PopID();

	ImGui::Text("Heightmap delta:");
	ImGui::SameLine();
	ImGui::PushID("delta");
	ImGui::DragFloat("", &myApp->m_render->delta, 0.00001f);
	ImGui::PopID();

	ImGui::Text("Light Direction:");
	ImGui::SameLine();
	ImGui::PushID("Light Dir");
	ImGui::DragFloat3("", myApp->m_render->lightDirection, 0.1f, -1.0f, 1.0f,"%.1f");
	ImGui::PopID();

	ImGui::Text("size = %d x %d", myApp->m_ui->textTest->width, myApp->m_ui->textTest->height);
	ImGui::Image((void*)(intptr_t)myApp->m_ui->textTest->imageID, ImVec2(500, 500));

	ImGui::End();

	return ret;
}
