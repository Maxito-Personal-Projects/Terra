#include "Application.h"
#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::Draw()
{
	bool ret = true;

	//Gennerating Vertec buffer id
	uint VBO;
	glGenBuffers(1, &VBO);

	//bind VB as array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copying the vertex data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertices, GL_STATIC_DRAW);

	return ret;
}
