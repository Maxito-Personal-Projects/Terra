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

	layerTypes[0] = 0;

	for (int i = 0; i < 7; ++i)
	{
		layerRanges[i] = 1.0f;
	}

	for (int i = 0; i < 6; ++i)
	{
		currTexture[i] = nullptr;
	}

	SetDefaultColors();
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
		canDrag = false;

		ImVec2 mainWindowSize = ImGui::GetWindowSize();

		float margin = 15.0f;

		float fill = (mainWindowSize.y-(175 + 270 + 170 + 130 + 170+50))/5.0f;

		char* primitives[] = { "Flat","Random","Perlin","Voronoi","Heightmap" };

		if (ImGui::GetScrollMaxY() > 0.0f)
		{
			margin = 30.0f;
		}

		ImVec2 windowSizes = { (mainWindowSize.x - margin) /* numWindows*/, /*mainWindowSize.y-*/250.0f };
		ImVec2 meshSize = { mainWindowSize.x - margin,175 + fill};
		ImVec2 biomeSize = { mainWindowSize.x - margin,270.0f + fill };
		if (heightWindow)
		{
			biomeSize.y *= 2.0f;
			biomeSize.y += 25.0f;
		}

		float finalSize = (numLayers-1) * 72.0f;

		ImVec2 layerSize = { mainWindowSize.x - margin,170.0f + fill + finalSize};
		ImVec2 chunkSize = { mainWindowSize.x - margin,130.0f +fill };
		ImVec2 oceanSize = { mainWindowSize.x - margin,170.0f + fill };

		ImVec4 titleColor = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);

		//--------------------------- Mesh Editor ------------------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize,2.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.5f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1.f, 1.f, 1.f, 1.0f));
		ImGui::BeginChild("Mesh Editor", meshSize, true);
		{
			//Centered Title
			std::string text = "Terrain Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.1f, 0.1f, 0.8f, 1.0f));

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
			ImGui::DragFloat("", &terrainHeight, 0.1f, 0.1f,400.0f);
			ImGui::PopID();

			ImGui::PushID("MeshWidth");
			ImGui::DragFloat("", &terrainWidth, 0.1f, 0.1f,400.0f);
			ImGui::PopID();

			ImGui::PushID("ChunkNum");
			ImGui::DragInt("", &terrainChunks,0.1f,1,100);
			ImGui::PopID();

			ImGui::EndGroup();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize("Generate").x + 30.0, ImGui::CalcTextSize("Generate").y + 14.0f);

			ImGui::SetCursorPos(ImVec2(windowSizes.x - buttonSize.x - 10.0f, ImGui::GetCursorPosY()));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
			ImGui::PushFont(myApp->m_ui->montserrat);
			if (ImGui::Button("Generate", buttonSize))
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
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.1f, 0.8f, 1.0f));
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
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
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
							currPrimitive = primitives[selectedBiome->primitive];
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

							terrain->parent->updateTFB = true;
						}
					}

					ImGui::EndCombo();
				}
				ImGui::PopFont();
				ImGui::PopID();

				ImGui::PushID("Terr Height");
				if (ImGui::DragFloat("", &selectedBiome->height, 1.0f, 0.0f))
				{
					terrain->parent->updateTFB = true;
					terrain->CalculateMaxBiomeHeight();
				}
				ImGui::PopID();

				ImGui::PushID("Seed");
				if (ImGui::DragFloat("", &selectedBiome->seed, 0.01f))
				{
					terrain->parent->updateTFB = true;
				}
				ImGui::PopID();

				ImGui::PushID("Octaves");
				if (ImGui::DragInt("", &selectedBiome->octaves, 0.1f, 1, 8, "%.0f"))
				{
					terrain->parent->updateTFB = true;
				}
				ImGui::PopID();

				if (currPrimitive != primitives[4])
				{
					ImGui::PushID("Frequency");
					if (ImGui::DragFloat("", &selectedBiome->frequency, 0.1, 0.1f, 1000.0f, "%.1f"))
					{
						terrain->parent->updateTFB = true;
					}
					ImGui::PopID();
				}

				ImGui::PushID("showBiome");
				if (ImGui::Checkbox("", &showBiome))
				{
					terrain->parent->updateTFB = true;
				}
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
					ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
					ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 20.0f, underLinePosL.y };
					drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.6f, 0.1f, 0.1f, 1.0f));


					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					char buff[256];
					strcpy_s(buff, 256, imageName.c_str());

					ImVec2 imageSize = { windowSizes.x - 75.0f,windowSizes.x - 75.0f };

					if (heightmap)
					{
						ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
						ImGui::Image((void*)(intptr_t)heightmap->imageID, ImVec2(imageSize));

						if (ImGui::IsItemHovered())
						{
							canDrag = true;
						}
					}
					else
					{
						ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
						ImGui::Image((void*)(intptr_t)myApp->m_ui->dragImage->imageID, ImVec2(imageSize));

						if (ImGui::IsItemHovered())
						{
							canDrag = true;
						}
					}

					ImGui::PushFont(myApp->m_ui->arial);
					ImGui::Text("File:");
					ImGui::PopFont();

					ImGui::SameLine();

					ImGui::PushID("Heightmapfile");
					ImGui::SetCursorPosX((ImGui::GetWindowSize().x - imageSize.x) / 2.f);
					ImGui::PushItemWidth(imageSize.x);
					ImGui::InputText("", buff, 256, ImGuiInputTextFlags_ReadOnly);
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
			}
			else
			{
				if (currBiome == "New Biome")
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

					if (ImGui::Button("Add Biome") && biomeName != "")
					{
						terrain->AddBiome(biomeName);
						selectedBiome = terrain->biomes[terrain->biomes.size() - 1];
						currBiome = biomeName;
						biomeName = "";
						currPrimitive = primitives[0];
					}
					ImGui::PopFont();
				}
			}
		}
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, .8f, 0.1f, 1.0f));
		ImGui::BeginChild("ChunkInfo", chunkSize, true);
		{
			//Centered Title
			std::string text = "Selected Chunk";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
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
							terrain->parent->updateTFB=true;
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

		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));
		ImGui::BeginChild("TextureEditor", oceanSize, true);
		{
			//Centered Title
			std::string text = "Texture Editor";
			ImGui::PushFont(myApp->m_ui->montserratBold);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
			ImGui::TextColored(titleColor, text.c_str());
			ImGui::PopFont();

			//Window DrawList
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			//Title Underline 
			ImVec2 underLinePosL = { ImGui::GetCursorPos().x + ImGui::GetWindowPos().x, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y };
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.0f, 0.7f, 0.7f, 1.0f));

			ImGui::PushFont(myApp->m_ui->arial);

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::BeginGroup();

			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Sand Texture:");

			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Grass Texture:");

			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Snow Texture:");

			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Rocks Texture:");

			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::PushID("Texture_1_A");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[0].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID("Texture_1_B");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[1].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::PushID("Texture_2_A");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[2].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID("Texture_2_B");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[3].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::PushID("Texture_3_A");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[6].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID("Texture_3_B");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[7].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::PushID("Texture_4_A");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[4].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID("Texture_4_B");
			ImGui::PushItemWidth(200.0f);
			ImGui::ColorEdit3("", layerColors[5].ptr(), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::EndGroup();

			ImGui::PopFont();
		}
		ImGui::EndChild();

		//----------------------- Layer Editor -----------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.1f, 1.0f));
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
			ImVec2 underLinePosR = { underLinePosL.x + ImGui::GetWindowSize().x - 18.0f, underLinePosL.y };
			drawList->AddLine(underLinePosL, underLinePosR, ImColor(0.6f, 0.6f, 0.0f, 1.0f));

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::PushFont(myApp->m_ui->arial);

			ImGui::BeginGroup();
			ImGui::Dummy(ImVec2(0.0f, 0.25f));
			ImGui::Text("Number of Layers:");
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::PushItemWidth(windowSizes.x - ImGui::CalcTextSize("Number of Layers:").x - 25.0f);
			ImGui::PushID("NumLayers");
			ImGui::SliderInt("", &numLayers, 1, 6);
			ImGui::PopID();

			ImGui::PopItemWidth();

			layerRanges[0] = 0.0f;
			layerRanges[6] = 1.0f;

			char* textures[] = { "Sand","Grass","Rock","Snow" };

			if (currTexture[0] == nullptr)
			{
				currTexture[0] = textures[0];
			}

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
						ImGui::PushID("Show 1");
						ImGui::PushItemWidth(75.0f);

						ImGui::DragFloat("", &layerRanges[i], 0.01f, 0.0f, 0.0, "%.2f");

						ImGui::PopItemWidth();
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);

						ImGui::DragFloat("", &layerRanges[i + 1], 0.01f, 0.0f, layerRanges[i + 2], "%.2f");

						ImGui::PopItemWidth();
						ImGui::PopID();

						if (numLayers == 1)
						{
							layerRanges[i] = 1.0f;
						}
					}
					else if (i > 0 && i < numLayers - 1)
					{
						string layerID = layer + "ID";

						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);
						ImGui::DragFloat("", &layerRanges[i], 0.01f, layerRanges[i - 1], layerRanges[i + 1], "%.2f");
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID(layerID.c_str());

						ImGui::DragFloat("", &layerRanges[i + 1], 0.01f, layerRanges[i], layerRanges[i + 2], "%.2f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (layerRanges[i] >= 1.0f)
						{
							layerRanges[i] = 1.0f;
							layerRanges[i + 1] = 1.0;
						}
					}
					else
					{
						ImGui::PushID(layer.c_str());
						ImGui::PushItemWidth(75.0f);

						ImGui::DragFloat("", &layerRanges[i], 0.01f, layerRanges[i - 1], 1.0, "%.2f");

						ImGui::PopItemWidth();
						ImGui::PopID();

						ImGui::SameLine();

						ImGui::PushID("Show last");
						ImGui::PushItemWidth(75.0f);
						ImGui::DragFloat("", &layerRanges[i + 1], 0.01f, 1.0, 1.0, "%.2f");
						ImGui::PopItemWidth();
						ImGui::PopID();
					}

					string type = layer + "type";

					ImGui::Text("Type: ");
					ImGui::SameLine();

					ImGui::PushID(type.c_str());
					if (ImGui::BeginCombo("", textures[layerTypes[i]]))
					{
						for (int j = 0; j < 4; ++j)
						{
							bool isSelected = (currTexture[i] == textures[j]);
							if (ImGui::Selectable(textures[j], isSelected))
							{
								currTexture[i] = textures[j];
								layerTypes[i] = j;
							}
						}

						ImGui::EndCombo();
					}

					ImGui::PopID();
				}
				else
				{
					layerTypes[i] = 0;
					layerRanges[i] = 1.0;
					currTexture[i] = textures[0];
				}

			}
			ImGui::PopFont();

			layerRanges[0] = 0.0f;
			layerRanges[6] = 1.0f;

		}
		ImGui::EndChild();

		ImGui::PopStyleColor(7);
		ImGui::PopStyleVar(5);
	}
	ImGui::End();

	if (askForDrag)
	{
		LoadHeightMap(pathToDrag);
		askForDrag = false;
		pathToDrag = "";
	}

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

		terrain->parent->updateTFB = true;
	}
}

void UIGeneration::SetDefaultColors()
{
	layerColors[0] = { 1.0f,1.0f,1.0f };
	layerColors[1] = { 1.0f,0.68f,0.06f };
	layerColors[2] = { 0.0f,0.3f,0.0f };
	layerColors[3] = { 0.24f,0.0f,0.0f };
	layerColors[4] = { 1.0f,1.0f,1.0f };
	layerColors[5] = { 0.0f,0.0f,0.0f };
	layerColors[6] = { 1.0f,1.0f,1.0f };
	layerColors[7] = { 0.02f,0.56f,0.51f };
}
