#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"
#include "Terrain.h"
#include "FileSystem.h"



Mesh::Mesh(GameObject* _parent, int x, int y)
{
	parent = _parent;
	chunkX = x;
	chunkY = y;

	GenerateFlatMesh_quads(x,y);
	FillInfoGPU();
	LoadToGPU();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &TBO);

	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}

	if (vertexBuffer)
	{
		delete[] vertexBuffer;
		vertexBuffer = nullptr;
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

	parent = nullptr;
}

void Mesh::DrawMesh()
{
	//GL_TEXTURE_0 is activate by default
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myApp->m_ui->textTest->imageID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, myApp->m_render->exportTexture);

	glBindVertexArray(VAO);

	// Render Info 
	int modelMatrix = glGetUniformLocation(parent->shader, "Model");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, *parent->transform->localMatrix.Transposed().v);
	int light = glGetUniformLocation(parent->shader, "lDirection");
	glUniform3f(light, myApp->m_render->lightDirection[0], myApp->m_render->lightDirection[1], myApp->m_render->lightDirection[2]);
	int delta = glGetUniformLocation(parent->shader, "delta");
	glUniform1f(delta, myApp->m_render->delta);
	int cam = glGetUniformLocation(parent->shader, "camPos");
	glUniform3f(cam, myApp->m_camera->mainCamera->GetPos().x, myApp->m_camera->mainCamera->GetPos().y, myApp->m_camera->mainCamera->GetPos().z);
	
	// Terrain info 
	int height = glGetUniformLocation(parent->shader, "maxHeight");
	glUniform1f(height, parent->terrain->maxHeight);
	int seed = glGetUniformLocation(parent->shader, "seed");
	glUniform1f(seed, parent->terrain->seed);
	int frequency = glGetUniformLocation(parent->shader, "freq");
	glUniform1f(frequency, parent->terrain->frequency);
	int octaves = glGetUniformLocation(parent->shader, "octaves");
	glUniform1i(octaves, parent->terrain->octaves);

	int brownian = glGetUniformLocation(parent->shader,"brownian_b");
	glUniform1i(brownian, parent->terrain->brownian);
	int perlin = glGetUniformLocation(parent->shader, "perlin_b");
	glUniform1i(perlin, parent->terrain->perlin);
	int heightmap = glGetUniformLocation(parent->shader, "heightmap_b");
	glUniform1i(heightmap, parent->terrain->heightmap);
	int voronoi = glGetUniformLocation(parent->shader, "voronoi_b");
	glUniform1i(voronoi, parent->terrain->voronoi);

	// Mesh info
	int grid = glGetUniformLocation(parent->shader, "gridSize");
	glUniform1i(grid, parent->terrain->numChunks);
	int div = glGetUniformLocation(parent->shader, "divisions");
	glUniform1f(div, divisions);
	int chunkCoords = glGetUniformLocation(parent->shader, "chunkCoords");
	glUniform2f(chunkCoords, chunkX, chunkY);

	// Other
	/*int testtime = glGetUniformLocation(parent->shader, "time");
	glUniform1f(testtime, time);*/

	//Binding transform feedback buffer
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,TBO);

	//Start Capturing shader output
	glBeginTransformFeedback(GL_TRIANGLES);
	
	//Drawing mesh
	glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, 0);

	//Stop Capturing shader output
	glEndTransformFeedback();

	time += 0.016;
}

void Mesh::LoadToGPU()
{
	//Generating Vertex Array, Vertex buffer, Index buffer & transform feedback buffer ids or names 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenBuffers(1, &TBO);

	//bind VA as array buffer first
	glBindVertexArray(VAO);

	//bind VB as array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copying the vertex data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 9, infoGPU, GL_STATIC_DRAW);

	//Binding the buffer to create it
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, TBO);

	//Allocating data for the Transform buffer //GL_DYNAMIC_COPY because we are going to change often and copy the data from the GPU
	//Size = division*division*numTris*numVertex*numFloats
	//If we want normals x2 (TODO) Done
	//If we want more tiles x(Tiles-1)^2 (TODO)
	buffSize = 64 * 64 * 2 * 3 * 3 * 2;
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(float) * 64 * 64 * 2 * 3 * 3 * 2, nullptr, GL_DYNAMIC_COPY);
	vertexBuffer = new float[buffSize];

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
	}
}

void Mesh::GenerateVertexBuffer()
{
	if (vertexBuffer)
	{
		glGetNamedBufferSubData(TBO, 0, sizeof(float)*buffSize, vertexBuffer);
	}
}

void Mesh::GenerateFlatMesh_quads(int x, int y)
{
	numVertices = 4;
	numIndices = 4;

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
			vertices[index * 3] = j * width + width*x;
			vertices[index * 3 + 1] = 0.0f;				
			vertices[index * 3 + 2] = (i * height) + height*y;

			if (j < size - 1 && i < size - 1)
			{
				testIndices[indice_it * 4] = index;
				testIndices[(indice_it * 4) + 1] = index + 1;
				testIndices[(indice_it * 4) + 2] = size + index+1;
				testIndices[(indice_it * 4) + 3] = size + index;
			}
		}
	}
}
