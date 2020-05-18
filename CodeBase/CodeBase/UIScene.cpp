#include "Application.h"
#include "ModuleRenderer.h"
#include "UIScene.h"
#include "ModuleUI.h"
#include "Texture.h"


UIScene::UIScene(std::string name, bool active) :UIWindow(name, active)
{
}

UIScene::~UIScene()
{
}

bool UIScene::Draw()
{
	bool ret = true;

	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
		if (ImGui::GetWindowSize().x != width || ImGui::GetWindowSize().y != height)
		{
			myApp->m_render->ResizeWindow(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
			
			width = ImGui::GetWindowSize().x;
			height = ImGui::GetWindowSize().y;
		}

		ImVec2 ImageSize = ImGui::GetWindowSize();
		ImageSize.x += ImGui::GetCursorPosX();
		ImageSize.y += ImGui::GetCursorPosY();

		ImGui::Image((void*)(intptr_t)myApp->m_render->fbTexture, ImageSize, { 0,1 }, { 1,0 });
	}
	ImGui::End();
	

	return ret;
}


