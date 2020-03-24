#include "Application.h"
#include "GameObject.h"
#include "ModuleShader.h"
#include "ModuleCamera.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"


GameObject::GameObject()
{
	transform = new Transform();
	mesh = new Mesh(this);

	//Default Shader
	shader = myApp->m_shader->GetShader("Default Shader");
}


GameObject::~GameObject()
{
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

bool GameObject::Draw()
{
	bool ret = true;

	if (transform)
	{
		
	}

	if (mesh)
	{
		//Using object shader
		glUseProgram(shader);
		SendMatrixToGPU();
		mesh->DrawMesh();
	}

	return ret;
}

void GameObject::SendMatrixToGPU()
{
	//Getting uniform from shader
	int projMatrix = glGetUniformLocation(shader, "Projection");
	//Sending info to the GPU
	glUniformMatrix4fv(projMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getProjectionMatrix());

	//Getting uniform from shader
	int viewMatrix = glGetUniformLocation(shader, "View");
	//Sending info to the GPU
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, myApp->m_camera->mainCamera->getViewMatrix());
}
