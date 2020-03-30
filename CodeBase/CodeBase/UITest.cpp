#include "Application.h"
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
	ImGui::Text("Que paxa!!!");
	ImGui::End();

	return ret;
}
