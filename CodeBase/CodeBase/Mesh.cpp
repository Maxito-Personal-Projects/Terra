#include "Application.h"
#include "ModuleShader.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"



Mesh::Mesh(GameObject* _parent)
{
	parent = _parent;
	//CalculateNormals();
	CalculateVertexNormals();
	FillInfoGPU();
	LoadToGPU();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	/*if (faceNormals)
	{
		delete[] faceNormals;
		faceNormals = nullptr;
	}*/

	if (infoGPU)
	{
		delete[] infoGPU;
		infoGPU = nullptr;
	}

	vertexNormals.clear();

	parent = nullptr;
}

void Mesh::DrawMesh()
{
	float lightPos[3] = { 0,10,-10 };
	glBindVertexArray(VAO);

	int modelMatrix = glGetUniformLocation(parent->shader, "Model");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, *parent->transform->localMatrix.Transposed().v);
	int light = glGetUniformLocation(parent->shader, "lightPos");
	glUniform3f(light, lightPos[0], lightPos[1], lightPos[2]);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 9, infoGPU, GL_STATIC_DRAW);

	//bind IB as element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Copying the index data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*numIndices, indices, GL_STATIC_DRAW);

	//Telling OpenGL how to interprete our data //Loading to GPU
	//( layout loactaion = 0, size of shader var -> vec3, type of data, normalize or not, space between data, offset)  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); //Position
	glEnableVertexAttribArray(0); //layout loactaion = 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float))); //Color
	glEnableVertexAttribArray(1); //layout loactaion = 1	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float))); //Normals
	glEnableVertexAttribArray(2); //layout loactaion = 2

	//safely unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//void Mesh::CalculateNormals()
//{
//	if (faceNormals)
//	{
//		delete[] faceNormals;
//		faceNormals = nullptr;
//	}
//
//	faceNormals = new float[numIndices * 2];
//	
//	for (int i = 0; i < numIndices; i = i + 3)
//	{
//		vec A = { verticesTest[indices[i] * 3],verticesTest[indices[i] * 3 + 1],verticesTest[indices[i] * 3 + 2] };
//		vec B = { verticesTest[indices[i + 1] * 3],verticesTest[indices[i + 1] * 3 + 1],verticesTest[indices[i + 1] * 3 + 2] };
//		vec C = { verticesTest[indices[i + 2] * 3],verticesTest[indices[i + 2] * 3 + 1],verticesTest[indices[i + 2] * 3 + 2] };
//
//		vec BA = A - B;
//		vec BC = C - B;
//
//		vec normal = BC.Cross(BA).Normalized();
//		
//		faceNormals[i] = normal.x;
//		faceNormals[i+1] = normal.y;
//		faceNormals[i+2] = normal.z;
//	}
//}

void Mesh::CalculateVertexNormals()
{
	vertexNormals.clear();

	for (int i = 0; i < numIndices; i = i + 3)
	{
		vec A = { verticesTest[indices[i] * 3],verticesTest[indices[i] * 3 + 1],verticesTest[indices[i] * 3 + 2] };
		vec B = { verticesTest[indices[i + 1] * 3],verticesTest[indices[i + 1] * 3 + 1],verticesTest[indices[i + 1] * 3 + 2] };
		vec C = { verticesTest[indices[i + 2] * 3],verticesTest[indices[i + 2] * 3 + 1],verticesTest[indices[i + 2] * 3 + 2] };

		vec BA = A - B;
		vec BC = C - B;

		vec normal = BC.Cross(BA).Normalized();

		std::map<int, vec>::iterator it;

		for (int j = 0; j < 3; ++j)
		{
			int key = indices[i + j];
			it = vertexNormals.find(key);

			if (it != vertexNormals.end())
			{
				vertexNormals[key] += normal;
			}
			else
			{
				vertexNormals[key] = normal;
			}
		}
	}

	std::map<int, vec>::iterator it = vertexNormals.begin();

	// Iterate over the map using Iterator till end.
	while (it != vertexNormals.end())
	{
		it->second = it->second.Normalized();
		it++;
	}
}

void Mesh::FillInfoGPU()
{
	if (infoGPU)
	{
		delete[] infoGPU;
		infoGPU = nullptr;
	}

	infoGPU = new float[numVertices*numProperties * 3];

	for (int i = 0; i < numVertices; i++)
	{
		int it = i * 9;
		int o_it = i * 3;

		//coping vertices
		infoGPU[it] = verticesTest[o_it];
		infoGPU[it + 1] = verticesTest[o_it+1];
		infoGPU[it + 2] = verticesTest[o_it+2];
		
		//coping Colors
		infoGPU[it + 3] = colors[o_it];
		infoGPU[it + 4] = colors[o_it + 1];
		infoGPU[it + 5] = colors[o_it + 2];
		
		//coping Normals
		infoGPU[it + 6] = vertexNormals[i].x;
		infoGPU[it + 7] = vertexNormals[i].y;
		infoGPU[it + 8] = vertexNormals[i].z;
	}


}
