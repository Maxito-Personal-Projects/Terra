#include "Application.h"
#include "ModuleUI.h"
#include "UIExport.h"
#include "ModuleRenderer.h"
#include "FileSystem.h"

#include "GameObject.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Mesh.h"



UIExport::UIExport(std::string name, bool active) :UIWindow(name, active)
{
	dae = true;
	format = DAE;
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

	if (ImGui::Button("Export", ImVec2(50, 50)))
	{
		if (fileName.length() > 0)
		{
			for (int i = 0; i < terrain->totalkChunks; ++i)
			{
				Mesh* mesh = terrain->chunks[i]->mesh;
				if (mesh->vertexBuffer)
				{
					//mesh->GenerateVertexBuffer();
					myApp->fileSystem->Export(mesh->vertexBuffer, mesh->buffSize, fileName + std::to_string(i), format,exportMessage);
				}
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