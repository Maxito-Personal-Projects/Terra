#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class Shader;

class GameObject
{
public:

	GameObject();
	~GameObject();

	bool Draw();

public:

	int shader = 0;
	float* vertices = nullptr;
	int numVertices = 0;

};

#endif // !__GAMEOBJECT_H__
