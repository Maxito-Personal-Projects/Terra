#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class Shader;
class Mesh;
class Transform;

class GameObject
{
public:

	GameObject();
	~GameObject();

	bool Draw();
	void SendMatrixToGPU();
	
public:

	int shader = 0;
	
	Mesh* mesh = nullptr;
	Transform* transform = nullptr;

};

#endif // !__GAMEOBJECT_H__
