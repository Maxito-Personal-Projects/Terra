#include "Application.h"
#include "ModuleShader.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"



Mesh::Mesh(GameObject* _parent)
{
	parent = _parent;
	LoadToGPU();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	parent = nullptr;
}

void Mesh::DrawMesh()
{	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);

	int modelMatrix = glGetUniformLocation(parent->shader, "Model");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, *parent->transform->localMatrix.Transposed().v);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::GenerateFlatMesh()
{
}

void Mesh::LoadToGPU()
{
	//Generating Vertex Array, Vertex buffer, Index buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//bind VA as array buffer first
	glBindVertexArray(VAO);

	//bind VB as array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copying the vertex data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 6, vertices, GL_STATIC_DRAW);

	//bind IB as element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Copying the index data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*numIndices, indices, GL_STATIC_DRAW);

	//Telling OpenGL how to interprete our data //Loading to GPU
	//( layout loactaion = 0, size of shader var -> vec3, type of data, normalize or not, space between data, offset)  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //layout loactaion = 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1); //layout loactaion = 1

	//safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
