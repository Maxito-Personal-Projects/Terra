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
	ImGui::Text("Render Settings:");
	ImGui::Text("Light Direction:");
	ImGui::SameLine();
	ImGui::PushID("Light Dir");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat3("", myApp->m_render->lightDirection, 0.1f, -1.0f, 1.0f, "%.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("Light delta:");
	ImGui::SameLine();
	ImGui::PushID("delta");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat("", &myApp->m_render->delta, 0.00001f, 0.000001f, 1.0f, "%.7f");
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

	ImGui::Text("-------------------------");

	// Terrain setting
	ImGui::Text("Terrain Settings:");
	
	ImGui::Text("Seed:");
	ImGui::SameLine();
	ImGui::PushID("Seed");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat("", &terrain->seed, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("Frequency:");
	ImGui::SameLine();
	ImGui::PushID("Frequency");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat("", &terrain->frequency, 0.1,0.1f, 1000.0f, "%.1f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("Octaves:");
	ImGui::SameLine();
	ImGui::PushID("Octaves");
	ImGui::PushItemWidth(100.0f);
	ImGui::DragInt("", &terrain->octaves, 0.1f, 1,8, "%.0f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::Text("-------------------------");


	ImGui::Text("-------------------------");

	//ImGui::Text("Map Generation:");
	//if (ImGui::Checkbox("Perlin Noise", &terrain->perlin))
	//{
	//	terrain->brownian = false;
	//	terrain->voronoi = false;
	//	terrain->heightmap = false;
	//}
	//if (ImGui::Checkbox("Brownian Noise", &terrain->brownian))
	//{
	//	terrain->perlin = false;
	//	terrain->voronoi = false;
	//	terrain->heightmap = false;
	//}
	//if (ImGui::Checkbox("Voronoi Noise", &terrain->voronoi))
	//{
	//	terrain->brownian = false;
	//	terrain->perlin = false;
	//	terrain->heightmap = false;
	//}
	//if (ImGui::Checkbox("Heightmap", &terrain->heightmap))
	//{
	//	terrain->brownian = false;
	//	terrain->voronoi = false;
	//	terrain->perlin = false;
	//}

	ImGui::Text("-------------------------");

	ImGui::Text("Result:");

	ImGui::Image((void*)(intptr_t)myApp->m_render->exportTexture, ImVec2(200, 200));


	ImGui::End();

	return ret;
}
