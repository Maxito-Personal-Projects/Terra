#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"



Mesh::Mesh(GameObject* _parent)
{
	parent = _parent;
	//GenerateFlatMesh_tris();
	GenerateFlatMesh_quads();
	//CalculateVertexNormals();
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

	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}

	if (testIndices)
	{
		delete[] testIndices;
		testIndices = nullptr;
	}

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
	//GL_TEXTURE_0 is activate by default
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myApp->m_ui->textTest->imageID);

	float lightPos[3] = { 0.1f,-1.0f,0.1f };
	glBindVertexArray(VAO);

	int modelMatrix = glGetUniformLocation(parent->shader, "Model");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, *parent->transform->localMatrix.Transposed().v);
	int light = glGetUniformLocation(parent->shader, "lightPos");
	glUniform3f(light, myApp->m_render->lightDirection[0], myApp->m_render->lightDirection[1], myApp->m_render->lightDirection[2]);
	int divisions = glGetUniformLocation(parent->shader, "divisions");
	glUniform1f(divisions, myApp->m_render->divisions);
	int height = glGetUniformLocation(parent->shader, "maxHeight");
	glUniform1f(height, myApp->m_render->maxHeight);
	int seed = glGetUniformLocation(parent->shader, "seed");
	glUniform1f(seed, myApp->m_render->seed);
	int delta = glGetUniformLocation(parent->shader, "delta");
	glUniform1f(delta, myApp->m_render->delta);
	int grid = glGetUniformLocation(parent->shader, "gridSize");
	glUniform1i(grid, size-1);

	int size = glGetUniformLocation(parent->shader, "freq");
	glUniform1i(size,myApp->m_render->size);
	int octaves = glGetUniformLocation(parent->shader, "octaves");
	glUniform1i(octaves, myApp->m_render->octaves);

	/*int testtime = glGetUniformLocation(parent->shader, "time");
	glUniform1f(testtime, time);*/

	glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, 0);

	time += 0.016;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*numIndices, testIndices, GL_STATIC_DRAW);

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
		int index = testIndices[i];
		int index_1 = testIndices[i+1];
		int index_2 = testIndices[i+2];

		vec A = { vertices[testIndices[i] * 3],	   vertices[testIndices[i] * 3 + 1],    vertices[testIndices[i] * 3 + 2] };
		vec B = { vertices[testIndices[i + 1] * 3],vertices[testIndices[i + 1] * 3 + 1],vertices[testIndices[i + 1] * 3 + 2] };
		vec C = { vertices[testIndices[i + 2] * 3],vertices[testIndices[i + 2] * 3 + 1],vertices[testIndices[i + 2] * 3 + 2] };

		vec BA = A - B;
		vec BC = C - B;

		vec normal = BC.Cross(BA).Normalized();

		std::map<int, vec>::iterator it;

		for (int j = 0; j < 3; ++j)
		{
			int key = testIndices[i + j];
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
	
	int size = numVertices * 9;

	infoGPU = new float[size];

	for (int i = 0; i < numVertices; i++)
	{
		int it = i * 9;
		int o_it = i * 3;
		int t_it = i * 2;

		//coping vertices
		infoGPU[it] = vertices[o_it];
		infoGPU[it + 1] = vertices[o_it+1];
		infoGPU[it + 2] = vertices[o_it+2];
		
		//coping Colors
		infoGPU[it + 3] = 0.5f;
		infoGPU[it + 4] = 0.5f;
		infoGPU[it + 5] = 0.5f;
		
		//coping Normals
		infoGPU[it + 6] = 0.0f;
		infoGPU[it + 7] = 1.0f;
		infoGPU[it + 8] = 0.0f;

		//Tile coords
		//infoGPU[it + 9] = (float)tileCoords[t_it];
		//infoGPU[it + 10] = (float)tileCoords[t_it+1];
	}
}

void Mesh::GenerateFlatMesh_tris()
{
	numVertices = size * size;
	numIndices = (size - 1)*(size - 1) * 6;

	vertices = new float[numVertices*3];
	testIndices = new int[numIndices];
	tileCoords = new int[numVertices * 2];

	int indice_it= 0;

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			//vertex index;
			int index = (i * size + j);

			//vertex position
			vertices[index * 3] = j * width;
			vertices[index * 3 + 1] = 0.0f;				//TODO let0s try to make this random :D
			vertices[index * 3 + 2] = (i * height);

			tileCoords[index * 2] = j;
			tileCoords[index * 2 + 1] = i;
			
			if (j < size - 1 && i < size - 1)
			{
				testIndices[indice_it * 6] = index;
				testIndices[(indice_it * 6) + 1] = index + 1;
				testIndices[(indice_it * 6) + 2] = size + index;
				testIndices[(indice_it * 6) + 3] = index + 1;
				testIndices[(indice_it * 6) + 4] = size + index + 1;
				testIndices[(indice_it * 6) + 5] = size + index;
				indice_it++;
			}
		}
	}
}

void Mesh::GenerateFlatMesh_quads()
{
	numVertices = size * size;
	numIndices = (size - 1)*(size - 1) * 4;

	vertices = new float[numVertices * 3];
	testIndices = new int[numIndices];

	int indice_it = 0;

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			//vertex index;
			int index = (i * size + j);

			//vertex position
			vertices[index * 3] = j * width;
			vertices[index * 3 + 1] = 0.0f;				
			vertices[index * 3 + 2] = (i * height);

			if (j < size - 1 && i < size - 1)
			{
				testIndices[indice_it * 4] = index;
				testIndices[(indice_it * 4) + 1] = index + 1;
				testIndices[(indice_it * 4) + 2] = size + index+1;
				testIndices[(indice_it * 4) + 3] = size + index;
				indice_it++;
			}
		}
	}
}
