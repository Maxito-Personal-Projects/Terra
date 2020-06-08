#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleUI.h"
#include "FileSystem.h"
#include "UIGeneration.h"

#include "GameObject.h"
#include "Texture.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Biome.h"

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

		ImVec2 mainWindowSize = ImGui::GetWindowSize();
		ImVec2 windowSizes = { (mainWindowSize.x-30.0f) /* numWindows*/, /*mainWindowSize.y-*/250.0f };
		ImVec2 meshSize = { mainWindowSize.x - 30.0f,175 };
		ImVec2 biomeSize = { mainWindowSize.x - 30.0f,270.0f };
		if (heightWindow)
		{
			biomeSize.y *= 2.0f;
			biomeSize.y += 25.0f;
		}
		ImVec2 layerSize = { mainWindowSize.x - 30.0f,250.0f};
		ImVec2 chunkSize = { mainWindowSize.x - 30.0f,130 };
		ImVec2 oceanSize = { mainWindowSize.x - 30.0f,100 };


		ImVec4 titleColor = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);

		//--------------------------- Mesh Editor ------------------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.8f, 0.5f));
		ImGui::BeginChild("Mesh Editor", meshSize, true);
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
				selectedChunk = nullptr;
				terrain->DeleteChunks();
				terrain->GenerateChunks(terrainChunks, terrainHeight, terrainWidth);
				terrain->SetNeighbours();
				terrain->parent->updateTFB = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(2);
			
		}
		ImGui::EndChild();

		//ImGui::SameLine();
		
		//--------------------------- Biome Editor ------------------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.1f, 0.8f, 0.5f));
		ImGui::BeginChild("BiomeSelector", biomeSize, true);
		{
			heightWindow = false;

			//Centered Title
			std::string text = "Biome Editor";
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

			//Biome Selector
			ImGui::PushFont(myApp->m_ui->arial);
			ImGui::Text("Biome:");

			ImGui::PushID("None Biome");
			if (ImGui::BeginCombo("", currBiome.c_str()))
			{
				for (int i = 0; i <= terrain->biomes.size(); ++i)
				{
					if (i == terrain->biomes.size())
					{
						bool isSelected = (currBiome == "New Biome");
						if (ImGui::Selectable("New Biome", isSelected))
						{
							currBiome = "New Biome";
							selectedBiome = nullptr;
							showBiome = false;
						}
					}
					else
					{
						bool isSelected = (currBiome == terrain->biomes[i]->name);
						if (ImGui::Selectable(terrain->biomes[i]->name.c_str(), isSelected))
						{
							currBiome = terrain->biomes[i]->name;
							selectedBiome = terrain->biomes[i];
						}
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopFont();
			ImGui::PopID();

			if (selectedBiome)
			{
				//type of primitives
				char* primitives[] = { "Flat","Random","Perlin","Voronoi","Heightmap" };

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

				ImGui::Dummy(ImVec2(0.0f, 0.05f));
				ImGui::Text("Show Biome");

				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();

				ImGui::Dummy(ImVec2(0.0f, 0.7f));

				ImGui::PushID("None Primitive");
				if (ImGui::BeginCombo("", primitives[selectedBiome->primitive]))
				{
					for (int i = 0; i < 5; ++i)
					{
						bool isSelected = (currPrimitive == primitives[i]);
						if (ImGui::Selectable(primitives[i], isSelected))
						{
							currPrimitive = primitives[i];
							selectedBiome->primitive = i;

							//TODO: implement Optimization!!!!
							terrain->parent->updateTFB = true;
						}
					}

					ImGui::EndCombo();
				}
				ImGui::PopFont();
				ImGui::PopID();

				ImGui::PushID("Terr Height");
				ImGui::DragFloat("", &selectedBiome->height, 1.0f, 0.0f);
				ImGui::PopID();

				ImGui::PushID("Seed");
				ImGui::DragFloat("", &selectedBiome->seed, 0.01f);
				ImGui::PopID();

				ImGui::PushID("Octaves");
				ImGui::DragInt("", &selectedBiome->octaves, 0.1f, 1, 8, "%.0f");
				ImGui::PopID();

				if (currPrimitive != primitives[4])
				{
					ImGui::PushID("Frequency");
					ImGui::DragFloat("", &selectedBiome->frequency, 0.1, 0.1f, 1000.0f, "%.1f");
					ImGui::PopID();
				}

				ImGui::PushID("showBiome");
				ImGui::Checkbox("",&showBiome);
				ImGui::PopID();

				ImGui::EndGroup();

				if (currPrimitive == primitives[4])
				{
					heightWindow = true;

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					//Centered Title
					std::string text = "Heightmap Selector";
					ImGui::PushFont(myApp->m_ui->montserratBold);
					ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
					ImGui::TextColored(titleColor, text.c_str());
					ImGui::PopFont();

					//Window DrawList
					ImDrawList* drawList = ImGui::GetWindowDrawList();

					//Title Underline 
					ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y +ImGui::GetWindowPos().y };
					ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x, underLinePosL.y };
					drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.6f, 0.1f, 0.1f, 1.0f));


					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					char buff[256];
					strcpy_s(buff, 256, imageName.c_str());

					if (heightmap)
					{
						ImVec2 imageSize = { windowSizes.x - 100.0f,windowSizes.x - 100.0f };
						ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
						ImGui::Image((void*)(intptr_t)heightmap->imageID, ImVec2(imageSize));

						if (ImGui::IsItemHovered())
						{
							canDrag = true;
						}
						else
						{
							canDrag = false;
						}
					}
					else
					{
						ImVec2 imageSize = { windowSizes.x - 100.0f,windowSizes.x - 100.0f };
						ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
						ImGui::Image((void*)(intptr_t)myApp->m_ui->dragImage->imageID, ImVec2(imageSize));

						if (ImGui::IsItemHovered())
						{
							canDrag = true;
						}
						else
						{
							canDrag = false;
						}
					}

					ImGui::PushFont(myApp->m_ui->arial);
					ImGui::Text("File:");
					ImGui::PopFont();

					ImGui::SameLine();

					ImGui::PushID("Heightmapfile");
					ImGui::InputText("", buff, 256, ImGuiInputTextFlags_ReadOnly);
					ImGui::PopID();
				}
			}
			else
			{
				if (currBiome=="New Biome")
				{
					char buff[256];
					strcpy_s(buff, 256, biomeName.c_str());

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::BeginGroup();
					ImGui::PushFont(myApp->m_ui->arial);
					ImGui::Text("Biome Name: ");
					ImGui::EndGroup();

					ImGui::SameLine();

					ImGui::Dummy(ImVec2(0.0f, -5.0f));
					ImGui::BeginGroup();

					ImGui::PushID("BiomeName");
					if (ImGui::InputText("", buff, 256))
					{
						biomeName = buff;
					}
					ImGui::PopID();
					ImGui::EndGroup();

					if (ImGui::Button("Add Biome"))
					{
						terrain->AddBiome(biomeName);
						selectedBiome = terrain->biomes[terrain->biomes.size() - 1];
						currBiome = biomeName;
						biomeName = "";
					}
					ImGui::PopFont();
				}
			}
		}
		ImGui::EndChild();		

		//ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, .8f, 0.1f, 0.5f));
		ImGui::BeginChild("ChunkInfo", chunkSize, true);
		{
			//Centered Title
			std::string text = "Chunk Info";
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

			if (selectedChunk)
			{
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				string chunkName = "Chunk ID: " + to_string(selectedChunk->chunkID);
				// Terrain setting
				ImGui::PushFont(myApp->m_ui->arial);
				ImGui::Text(chunkName.c_str());

				string chunkBiome = "Biome: ";

				ImGui::Text(chunkBiome.c_str());

				ImGui::SameLine();

				ImGui::PushID("Chunk Biome");
				if (ImGui::BeginCombo("", selectedChunk->biome->name.c_str()))
				{
					for (int i = 0; i < terrain->biomes.size(); ++i)
					{
						bool isSelected = (selectedChunk->biome->name == terrain->biomes[i]->name);
						if (ImGui::Selectable(terrain->biomes[i]->name.c_str(), isSelected))
						{
							selectedChunk->biome = terrain->biomes[i];
						}
					}

					ImGui::EndCombo();
				}
				ImGui::PopID();

				string chunkCoords = "Coord X:  " + to_string(selectedChunk->x) + " Coord Y:  " + to_string(selectedChunk->y);
				// Terrain setting
				ImGui::Text(chunkCoords.c_str());
				ImGui::PopFont();

			}
		}
		ImGui::EndChild();

		//ImGui::SameLine();

		//----------------------- Layer Editor -----------------------------------------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.8f, 0.1f, 0.5f));
		ImGui::BeginChild("Layer Editor", layerSize, true);
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

		//ImGui::SameLine();

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


void UIGeneration::LoadHeightMap(string path)
{
	if (canDrag)
	{
		if (heightmap)
		{
			glDeleteTextures(1, &(heightmap->imageID));
			delete heightmap;
			heightmap = nullptr;
		}

		imageName = myApp->fileSystem->GetFileNameFromPath(path);
		heightmap = myApp->fileSystem->LoadImagePNG(path);
	}
}