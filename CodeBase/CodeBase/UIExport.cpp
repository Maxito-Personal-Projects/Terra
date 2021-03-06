#include "Application.h"
#include "ModuleUI.h"
#include "UIExport.h"
#include "ModuleRenderer.h"
#include "FileSystem.h"

#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"

#include <direct.h>


UIExport::UIExport(std::string name, bool active) :UIWindow(name, active)
{
	dae = true;
	format = DAE;

	currResolution = "High";
}

UIExport::~UIExport()
{
}

bool UIExport::Draw()
{
	if (!terrain && myApp->m_render->firstGO)
	{
		terrain = myApp->m_render->firstGO->terrain;
	}

	bool ret = true;

	ImGui::Begin(name.c_str(), &active);

	char buff[256];
	strcpy_s(buff, 256, fileName.c_str());

	ImGui::Text("File Name: ");
	ImGui::SameLine();
	ImGui::PushID("FileName");
	if (ImGui::InputText("", buff, 256))
	{
		fileName = buff;
	}
	ImGui::PopID();


	if (ImGui::Checkbox(".dae", &dae))
	{
		dae = true;
		obj_m = false;
		obj = false;
		fbx = false;

		format = DAE;
		exportMessage = "";
	}
	ImGui::SameLine();
	if (ImGui::Checkbox(".obj (mat)", &obj_m))
	{
		obj_m = true;
		dae = false;
		obj = false;
		fbx = false;

		format = OBJ_MAT;
		exportMessage = "";
	}

	if (ImGui::Checkbox(".obj", &obj))
	{
		obj = true;
		dae = false;
		obj_m = false;
		fbx = false;

		format = OBJ;
		exportMessage = "";
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("fbx (not workig)", &fbx))
	{
		fbx = true;

		dae = false;
		obj_m = false;
		obj = false;

		format = FBX;
		exportMessage = "";
	}

	ImGui::Text("Resolution:");

	ImGui::SameLine();

	char* resolutions[] = { "High","Mid","Low"};

	ImGui::PushID("None Function");
	if (ImGui::BeginCombo("", currResolution))
	{
		for (int i = 0; i < 3; ++i)
		{
			bool isSelected = (currResolution == resolutions[i]);
			if (ImGui::Selectable(resolutions[i], isSelected))
			{
				currResolution = resolutions[i];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	ImGui::Text("Export Texture:");

	ImGui::SameLine();

	ImGui::PushID("ExportTexture");
	ImGui::Checkbox("", &texture);
	ImGui::PopID();

	if (ImGui::Button("Export", ImVec2(50, 50)))
	{
		exportMessage = "";

		if (fileName.length() > 0)
		{
			string path = "Exports\\" + fileName;
			_mkdir(path.c_str());

			for (int i = 0; i < terrain->totalkChunks; ++i)
			{
				Mesh* mesh = terrain->chunks[i]->mesh;

				if (currResolution == "High")
				{
					if (mesh->vertexBuffer64)
					{
						if(texture)
							mesh->DrawTextureToExport(fileName + "\\" + fileName + std::to_string(i));

						mesh->GenerateVertexBuffer();
						myApp->fileSystem->Export(mesh->vertexBuffer64, mesh->buffSize64, fileName + "\\" + fileName + "_" + std::to_string(i)+"_64", format, exportMessage);
					}
				}
				else if (currResolution == "Mid")
				{
					terrain->chunks[i]->mesh->divisions = 32.0f;
					terrain->chunks[i]->mesh->DrawMesh(true, false);

					if (mesh->vertexBuffer32)
					{
						if (texture)
							mesh->DrawTextureToExport(fileName + "\\" + fileName + std::to_string(i));

						mesh->GenerateVertexBuffer();
						myApp->fileSystem->Export(mesh->vertexBuffer32, mesh->buffSize32, fileName + "\\" + fileName + "_" + std::to_string(i) + "_32", format, exportMessage);
					}
				}
				else if(currResolution=="Low")
				{
					terrain->chunks[i]->mesh->divisions = 16.0f;
					terrain->chunks[i]->mesh->DrawMesh(true, false);

					if (mesh->vertexBuffer16)
					{
						if (texture)
							mesh->DrawTextureToExport(fileName + "\\" + fileName + std::to_string(i));

						mesh->GenerateVertexBuffer();
						myApp->fileSystem->Export(mesh->vertexBuffer16, mesh->buffSize16, fileName + "\\" + fileName + "_" + std::to_string(i) + "_16", format, exportMessage);
					}
				}

				terrain->chunks[i]->mesh->divisions = 64.0f;
			}
		}
		else
		{
			exportMessage = "Add name before exporting";
		}
	}

	ImGui::Text(exportMessage.c_str());
	ImGui::End();

	return ret;
}