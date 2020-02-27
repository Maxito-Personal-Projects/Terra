#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class Shader;

class GameObject
{
public:

	GameObject();
	~GameObject();

	bool Draw();
	
	void LoadToGPU();

public:

	int shader = 0;
	float vertices[12] = { 0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,-0.5f, -0.5f, 0.0f,-0.5f,  0.5f, 0.0f };
	int indices[6] = { 0,1,3,1,2,3 };
	int numVertices = 4;
	int numIndices = 6;

	uint VBO;
	uint VAO;
	uint IBO;

};

#endif // !__GAMEOBJECT_H__
