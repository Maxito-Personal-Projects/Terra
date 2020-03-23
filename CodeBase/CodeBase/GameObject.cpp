#include "Application.h"
#include "GameObject.h"
#include "Mesh.h"
#include "ModuleShader.h"
#include "Transform.h"

GameObject::GameObject()
{
	transform = new Transform();
	mesh = new Mesh();
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
		//glUseProgram(shader); should be like this but now
		glUseProgram(myApp->m_shader->GetShader("Default Shader"));
		mesh->DrawMesh();
	}

	return ret;
}
