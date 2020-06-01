#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleUI.h"
#include "FileSystem.h"
#include "UIGeneration.h"

#include "GameObject.h"
#include "Texture.h"
#include "Terrain.h"
#include "Chunk.h"

#include <Windows.h>

UIGeneration::UIGeneration(std::string name, bool active) : UIWindow(name, active)
{
	currPrimitive = "None";
	currFunction = "None";
	currNumLayer = "1";

	layerRanges[0] = { 0.0f,1.0f };
	layerColors[0] = { 0.25f,0.88f,0.82f};
}


UIGeneration::~UIGeneration()
{
}

bool UIGeneration::Draw()
{
	bool ret = true;

	if (!terrain)
	{
		terrain = myApp->m_render->firstGO->terrain;
		terrainWidth = terrain->width;
		terrainHeight = terrain->height;
		terrainChunks = terrain->numChunks;
	}

	ImGui::Begin(name.c_str(), &active);
	{
		int numWindows = 6;

		if (heightWindow) numWindows = 5;

		ImVec2 mainWindowSize = ImGui::GetWindowSize();
		ImVec2 windowSizes = { (mainWindowSize.x-50.0f)/ numWindows, mainWindowSize.y-50.0f };

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

			ImGui::PushID("MeshHeight");
			ImGui::DragFloat("", &terrainHeight);
			ImGui::PopID();

			ImGui::PushID("MeshWidth");
			ImGui::DragFloat("", &terrainWidth);
			ImGui::PopID();

			ImGui::PushID("ChunkNum");
			ImGui::DragInt("", &terrainChunks);
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
				terrain->DeleteChunks();
				terrain->GenerateChunks(terrainChunks, terrainHeight, terrainWidth);
				terrain->SetNeighbours();
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
			heightWindow = false;

			//type of primitives
			char* primitives[] = { "Flat","Random","Perlin","Voronoi","Heightmap" };

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
			ImGui::Dummy(ImVec2(0.0f, 0.1f));
			ImGui::Text("Height:");
			ImGui::Dummy(ImVec2(0.0f, 0.05f));
			ImGui::Text("Seed:");
			ImGui::Dummy(ImVec2(0.0f, 0.05f));
			ImGui::Text("Octaves:");

			if (currPrimitive != primitives[4])
			{
				ImGui::Dummy(ImVec2(0.0f, 0.05f));
				ImGui::Text("Frequency:");
			}

			if (currPrimitive == primitives[4])
			{
				char buff[256];
				strcpy_s(buff, 256, fileName.c_str());

				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text("File:");
				ImGui::PopFont();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::PushID("None Primitive");
			if (ImGui::BeginCombo("", currPrimitive))
			{
				for (int i = 0; i < 5; ++i)
				{
					bool isSelected = (currPrimitive == primitives[i]);
					if (ImGui::Selectable(primitives[i], isSelected))
					{
						currPrimitive=primitives[i];
						terrain->primitive = i;
					}
				}

				terrain->parent->updateTFB = true;

				ImGui::EndCombo();
			}
			ImGui::PopFont();
			ImGui::PopID();

			ImGui::PushID("Terr Height");
			ImGui::DragFloat("", &terrain->maxHeight, 1.0f, 0.0f);
			ImGui::PopID();

			ImGui::PushID("Seed");
			ImGui::DragFloat("", &terrain->seed, 0.01f);
			ImGui::PopID();

			ImGui::PushID("Octaves");
			ImGui::DragInt("", &terrain->octaves, 0.1f, 1, 8, "%.0f");
			ImGui::PopID();

			if (currPrimitive != primitives[4])
			{
				ImGui::PushID("Frequency");
				ImGui::DragFloat("", &terrain->frequency, 0.1, 0.1f, 1000.0f, "%.1f");
				ImGui::PopID();
			}

			if (currPrimitive == primitives[4])
			{
				heightWindow = true;

				char buff[256];
				strcpy_s(buff, 256, fileName.c_str());

				ImGui::PushID("Heightmapfile");
				ImGui::InputText("", buff, 256, ImGuiInputTextFlags_ReadOnly);
				ImGui::PopID();

				char buff2[256];

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
				if (ImGui::Button("...", ImVec2(50.0f, 25.0f)))
				{
					string ret = myApp->fileSystem->GetFileNameAt(myApp->fileSystem->GetFolderPath("Images").c_str());
					if (ret != "")
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
					ImVec2 imageSize = { windowSizes.x - 25.0f,windowSizes.x - 25.0f };
					ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
					ImGui::Image((void*)(intptr_t)heightmap->imageID, ImVec2(imageSize));
				}
			}

			ImGui::EndGroup();


		}
		ImGui::EndChild();

		//--------------------------- Function Editor ------------------------------------------------
		if (!heightWindow)
		{
			ImGui::SameLine();

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

				char* functions[] = { "Brownian Motion","Fraction","Module", "Sinus" };

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
			ImGui::PopStyleColor(1);
		}

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

			for (int i = 0; i < 6; i++)
			{
				if (i < numLayers)
				{
					string layer = "Layer " + std::to_string(i + 1);
					ImGui::Text(layer.c_str());

					ImGui::Text("Range: ");

					ImGui::SameLine();

					if (i == 0)
					{
						layerRanges[i].x = 0.0f;

						ImGui::PushID("Show 1");
						ImGui::PushItemWidth(75.0f);
						ImGui::DragFloat("", &layerRanges[i].x, 0.01f, 0.0f, 1.0, "%.2f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);
						if (ImGui::DragFloat("", &layerRanges[i].ptr()[1], 0.01f, 0.0f, 1.0, "%.2f"))
						{
							layerRanges[i + 1].x = layerRanges[i].y;
						}
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (numLayers==1)
						{
							layerRanges[i].y = 1.0f;
						}
					}
					else if(i>0 && i<numLayers-1)
					{
						string layerID = layer + "ID";

						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);
						if (ImGui::DragFloat("", &layerRanges[i - 1].ptr()[1], 0.01f, layerRanges[i-1].x, layerRanges[i].y, "%.2f"))
						{
							layerRanges[i].x = layerRanges[i-1].y;
						}
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID(layerID.c_str());

						if (ImGui::DragFloat("", &layerRanges[i].ptr()[1], 0.01f, layerRanges[i].x, layerRanges[i+1].y, "%.2f") && i < numLayers - 1)
						{
							layerRanges[i+1].x = layerRanges[i].y;
						}
						ImGui::PopItemWidth();
						ImGui::PopID();
					}
					else
					{
						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);
						if (ImGui::DragFloat("", &layerRanges[i].ptr()[0], 0.01f, layerRanges[i - 1].x, 1.0, "%.2f"))
						{
							layerRanges[i-1].y = layerRanges[i].x;
						}
						ImGui::PopItemWidth();
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID("Show last");
						ImGui::PushItemWidth(75.0f);
						ImGui::DragFloat("", &layerRanges[i].y, 0.01f, 0.0f, 1.0, "%.2f");
						ImGui::PopItemWidth();
						ImGui::PopID();


						layerRanges[i].y = 1.0f;
					}

					ImGui::Text("Color: ");

					ImGui::SameLine();

					string Color = layer + "color";
					
					ImGui::PushID(Color.c_str());
					ImGui::PushItemWidth(200.0f);
					ImGui::ColorEdit3("", layerColors[i].ptr());
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
				else
				{
					layerRanges[i] = { 1.0f,1.0f };
					layerColors[i] = { 0.0f,0.0f,0.0f };
				}

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

			if (selectedChunk)
			{
				string chunkName = "Chunk " + to_string(selectedChunk->chunkID) + " Settings";
				// Terrain setting
				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text(chunkName.c_str());
				ImGui::PopFont();


				string chunkCoords = "Coord X:  " + to_string(selectedChunk->x) + " Coord Y:  " + to_string(selectedChunk->y);
				// Terrain setting
				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text(chunkCoords.c_str());
				ImGui::PopFont();

				ImGui::BeginGroup();
				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text("Chunk Height:");
				ImGui::Text("Octaves:");
				ImGui::PopFont();
				ImGui::EndGroup();

				
				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::PushID("C Height");
				ImGui::DragFloat("", &selectedChunk->maxHeight, 1.0f, 0.0f);
				ImGui::PopID();
				
				ImGui::PushID("C Octaves");
				ImGui::DragInt("", &selectedChunk->octaves, 0.1f, 1, 8, "%.0f");
				ImGui::PopID();
				ImGui::EndGroup();

			}
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

		ImGui::PopStyleColor(5);
	}
	ImGui::End();

	return ret;
}