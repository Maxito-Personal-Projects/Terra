#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleUI.h"
#include "FileSystem.h"
#include "UIGeneration.h"
#include "Texture.h"
#include <Windows.h>

UIGeneration::UIGeneration(std::string name, bool active) : UIWindow(name, active)
{
	currPrimitive = "None";
	currFunction = "None";
	currNumLayer = "1";
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

		ImVec4 titleColor = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);

		//--------------------------- Mesh Editor ------------------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.8f, 0.5f));
		ImGui::BeginChild("Mesh Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Mesh Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x-ImGui::CalcTextSize(text.c_str()).x)/2.f);
			ImGui::TextColored(titleColor,text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y};
			drawList->AddLine(underLinePosL,underLinePosR,ImColor(0.1f, 0.1f, 0.8f, 1.0f));

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::BeginGroup();
			ImGui::PushFont(myApp->m_ui->arial);

			ImGui::Text("Height:");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Width:");
			ImGui::Dummy(ImVec2(0.0f, 0.25f));
			ImGui::Text("Chunks:");

			ImGui::PopFont();
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			float ftest = 0;
			int itest = 0;

			ImGui::PushID("MeshHeight");
			ImGui::DragFloat("", &ftest);
			ImGui::PopID();

			ImGui::PushID("MeshWidth");
			ImGui::DragFloat("", &ftest);
			ImGui::PopID();

			ImGui::PushID("ChunkNum");
			ImGui::DragInt("", &itest);
			ImGui::PopID();

			ImGui::EndGroup();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize("Generate").x + 30.0, ImGui::CalcTextSize("Generate").y + 14.0f);

			ImGui::SetCursorPos(ImVec2(windowSizes.x - buttonSize.x-10.0f, ImGui::GetCursorPosY()));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
			ImGui::PushFont(myApp->m_ui->montserrat);
			if (ImGui::Button("Generate",buttonSize))
			{
				LOG("Mesh Generated");
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(2);
			
		}
		ImGui::EndChild();

		ImGui::SameLine();
		
		//--------------------------- Primitive Editor ------------------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.1f, 0.8f, 0.5f));
		ImGui::BeginChild("Primitive Selector", windowSizes, true);
		{
			//Centered Title
			std::string text = "Primitive Selector";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.8f, 0.1f, 0.8f, 1.0f));

			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			
			ImGui::BeginGroup();
			ImGui::Dummy(ImVec2(0.0f, 0.25f));
			ImGui::PushFont(myApp->m_ui->arial);
			ImGui::Text("Primitive:");
			ImGui::EndGroup();

			ImGui::SameLine();

			char* primitives[] = { "Flat","Perlin","Voronoi", "Random","Heightmap" };
			
			ImGui::PushID("None Primitive");
			if (ImGui::BeginCombo("", currPrimitive))
			{
				for (int i = 0; i < 5; ++i)
				{
					bool isSelected = (currPrimitive == primitives[i]);
					if (ImGui::Selectable(primitives[i], isSelected))
					{
						currPrimitive=primitives[i];
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopFont();
			ImGui::PopID();

			if (currPrimitive == primitives[4])
			{
				char buff[256];
				strcpy_s(buff, 256, fileName.c_str());

				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text("File:");
				ImGui::PopFont();
				
				ImGui::SameLine();

				ImGui::PushID("Heightmapfile");
				ImGui::InputText("", buff, 256,ImGuiInputTextFlags_ReadOnly);
				ImGui::PopID();

				char buff2[256];

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
				if (ImGui::Button("...",ImVec2(50.0f,25.0f)))
				{
					string ret = myApp->fileSystem->GetFileNameAt(myApp->fileSystem->GetFolderPath("Images").c_str());
					if (ret!="")
					{
						if (heightmap)
						{
							glDeleteTextures(1, &(heightmap->imageID));
							delete heightmap;
							heightmap = nullptr;
						}

						fileName = myApp->fileSystem->GetFileNameFromPath(ret);
						heightmap = myApp->fileSystem->LoadImagePNG(ret);
					}
				}
				ImGui::PopStyleColor(2);

				if (heightmap)
				{
					ImVec2 imageSize = {windowSizes.x-25.0f,windowSizes.x - 25.0f };
					ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
					ImGui::Image((void*)(intptr_t)heightmap->imageID, ImVec2(imageSize));
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		//--------------------------- Function Editor ------------------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.1f, 0.1f, 0.5f));
		ImGui::BeginChild("Function Selector", windowSizes, true);
		{
			//Centered Title
			std::string text = "Function Selector";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.6f, 0.1f, 0.1f, 1.0f));


			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::BeginGroup();
			ImGui::Dummy(ImVec2(0.0f, 0.25f));
			ImGui::PushFont(myApp->m_ui->arial);
			ImGui::Text("Function:");
			ImGui::EndGroup();

			ImGui::SameLine();

			char* functions[] = { "Brownian Motion","Fraction","Module", "Sinus"};

			ImGui::PushID("None Function");
			if (ImGui::BeginCombo("", currFunction))
			{
				for (int i = 0; i < 4; ++i)
				{
					bool isSelected = (currFunction == functions[i]);
					if (ImGui::Selectable(functions[i], isSelected))
					{
						currFunction = functions[i];
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopFont();
			ImGui::PopID();

		}
		ImGui::EndChild();

		ImGui::SameLine();

		//----------------------- Layer Editor -----------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.8f, 0.1f, 0.5f));
		ImGui::BeginChild("Layer Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Layer Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.6f, 0.6f, 0.0f, 1.0f));

			ImGui::Dummy(ImVec2(0.0f,10.0f));

			ImGui::BeginGroup();
			ImGui::Dummy(ImVec2(0.0f, 0.25f));
			ImGui::PushFont(myApp->m_ui->arial);
			ImGui::Text("Number of Layers:");
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::PushItemWidth(windowSizes.x - ImGui::CalcTextSize("Number of Layers:").x-25.0f);
			ImGui::PushID("NumLayers");
			ImGui::SliderInt("",&numLayers,1,6);
			ImGui::PopID();

			ImGui::PopItemWidth();

			for (int i = 0; i < numLayers; i++)
			{
				string layer = "Layer " + std::to_string(i+1);
				ImGui::Text(layer.c_str());

				float range[2] = { 0.0f,1.0f };
				float color[3];
				
				ImGui::Text("Range: ");

				ImGui::SameLine();

				ImGui::PushItemWidth(75.0f);
				ImGui::DragFloat2("",range,0.01f,0.0f,1.0,"%.2f");
				ImGui::PopItemWidth();

				ImGui::SameLine();

				ImGui::PushItemWidth(150.0f);
				ImGui::ColorEdit3("", color);
				ImGui::PopItemWidth();

			}
			ImGui::PopFont();


		}
		ImGui::EndChild();
		
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, .8f, 0.1f, 0.5f));
		ImGui::BeginChild("Biomes Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Biomes Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.0f, 0.7f, 0.0f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.8f, 0.8f, 0.5f));
		ImGui::BeginChild("Ocean Editor", windowSizes, true);
		{
			//Centered Title
			std::string text = "Ocean Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.0f, 0.7f, 0.7f, 1.0f));

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::PopStyleColor(6);
	}
	ImGui::End();

	return ret;
}