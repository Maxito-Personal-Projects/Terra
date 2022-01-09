#include "Application.h"
#include "GameObject.h"
#include "ModuleShader.h"
#include "ModuleCamera.h"
#include "ModuleRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Terrain.h"


GameObject::GameObject()
{
	transform = new Transform();
	terrain = new Terrain(this,1);

	if (terrain)
	{
		terrain->SetNeighbours();
	}

	//Default Shader
	terrainShader = myApp->m_shader->GetShader("Default Shader");
	renderShader = myApp->m_shader->GetShader("Render Shader");
	mousePickingShader = myApp->m_shader->GetShader("Mouse Picking Shader");
	textureShader = myApp->m_shader->GetShader("Texture Shader");

	updateTFB = true;
}


GameObject::~GameObject()
{
	if (transform)
	{
		delete transform;
		transform = nullptr;
	}
	if (terrain)
	{
		delete terrain;
		terrain = nullptr;
	}
}

bool GameObject::Draw()
{
	bool ret = true;

	if (terrain)
	{
		if (isLoading)
		{
			isLoading = false;
			terrain->Load();
		}
		else
		{
			if (updateTFB)
			{
				glUseProgram(terrainShader);
				SendMatrixToGPU(terrainShader);
				terrain->DrawChunks(updateTFB);
				if (myApp->m_render->optim)
				{
					updateTFB = false;
				}
			}
			else
			{
				glUseProgram(renderShader);
				SendMatrixToGPU(renderShader);
				terrain->DrawChunks(updateTFB);
			}
		}
	}

	return ret;
}

bool GameObject::SelectionDraw()
{
	bool ret = true;

	if (terrain)
	{
		//Using object shader
		glUseProgram(mousePickingShader);
		SendMatrixToGPU(mousePickingShader);
		terrain->DrawSelectionChunks();
	}

	return ret;
}

void GameObject::SendMatrixToGPU(int shaderID)
{
	float4x4 identity = float4x4::identity;

	//Getting uniform from shader
	int projMatrix = glGetUniformLocation(shaderID, "Projection");
	//Sending info to the GPU
	glUniformMatrix4fv(projMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getProjectionMatrix());

	//Getting uniform from shader
	int viewMatrix = glGetUniformLocation(shaderID, "View");
	//Sending info to the GPU
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getViewMatrix());
}
