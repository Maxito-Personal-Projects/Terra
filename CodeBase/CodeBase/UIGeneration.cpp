#include "Application.h"
#include "ModuleUI.h"
#include "UIGeneration.h"

UIGeneration::UIGeneration(std::string name, bool active) : UIWindow(name, active)
{
}


UIGeneration::~UIGeneration()
{
}

bool UIGeneration::Draw()
{
	bool ret = true;

	ImGui::Begin(name.c_str(), &active);
	{
		ImVec2 mainWindowSize = ImGui::GetWindowSize();
		ImVec2 windowSizes = { (mainWindowSize.x-50.0f)/6.f, mainWindowSize.y-50.0f };

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.5f, 0.5f, 1.0f, 0.5f));
		ImGui::BeginChild("Mesh Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Mesh Editor";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x-ImGui::CalcTextSize(text.c_str()).x)/2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f),text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y};
			drawList->AddLine(underLinePosL,underLinePosR,ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();

			float test = 0;
			ImGui::DragFloat("Mesh Width:", &test);
			
		}
		ImGui::EndChild();

		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.5f, 1.0f, 0.5f));
		ImGui::BeginChild("Primitive Selector", windowSizes, true);
		{
			//Centered Title
			std::string text = "Primitive Selector";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.5f, 0.5f, 0.5f));
		ImGui::BeginChild("Function Selector", windowSizes, true);
		{
			//Centered Title
			std::string text = "Function Selector";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 0.5f, 0.5f));
		ImGui::BeginChild("Layer Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Layer Editor";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();
		
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.5f, 1.0f, 0.5f, 0.5f));
		ImGui::BeginChild("Biomes Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Biomes Editor";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.5f, 1.0f, 1.0f, 0.5f));
		ImGui::BeginChild("Ocean Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Ocean Editor";
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), text.c_str());

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.5f, 0.5f, 1.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::PopStyleColor(6);
	}
	ImGui::End();

	return ret;
}