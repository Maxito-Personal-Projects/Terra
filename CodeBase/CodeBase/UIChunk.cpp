#include "ModuleUI.h"
#include "UIChunk.h"
#include "Chunk.h"



UIChunk::UIChunk(std::string name, bool active) :UIWindow(name, active)
{
}


UIChunk::~UIChunk()
{
}

bool UIChunk::Draw()
{
	bool ret = true;

	if (selectedChunk)
	{
		ImGui::Begin(name.c_str(),&active);
		{
			// Terrain setting
			ImGui::Text("Terrain Settings:");

			ImGui::Text("Chunk Height:");
			ImGui::SameLine();
			ImGui::PushID("Height");
			ImGui::PushItemWidth(100.0f);
			ImGui::DragFloat("", &selectedChunk->maxHeight, 1.0f, 0.0f);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::Text("Octaves:");
			ImGui::SameLine();
			ImGui::PushID("Octaves");
			ImGui::PushItemWidth(100.0f);
			ImGui::DragInt("", &selectedChunk->octaves, 0.1f, 1, 8, "%.0f");
			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		ImGui::End();
	}

	return ret;
}
