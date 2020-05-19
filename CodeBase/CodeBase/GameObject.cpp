#include "Application.h"
#include "GameObject.h"
#include "ModuleShader.h"
#include "ModuleCamera.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Terrain.h"


GameObject::GameObject()
{
	transform = new Transform();
	terrain = new Terrain(this,2);

	//Default Shader
	shader = myApp->m_shader->GetShader("Default Shader");
	mousePickingShader = myApp->m_shader->GetShader("Mouse Picking Shader");
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
		//Using object shader
		glUseProgram(shader);
		SendMatrixToGPU(shader);
		terrain->DrawChunks();
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
	//Getting uniform from shader
	int projMatrix = glGetUniformLocation(shaderID, "Projection");
	//Sending info to the GPU
	glUniformMatrix4fv(projMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getProjectionMatrix());

	//Getting uniform from shader
	int viewMatrix = glGetUniformLocation(shaderID, "View");
	//Sending info to the GPU
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getViewMatrix());
}
