#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class Shader;
class Mesh;
class Transform;
class Terrain;

class GameObject
{
public:

	GameObject();
	~GameObject();

	bool Draw();
	bool SelectionDraw();
	void SendMatrixToGPU(int shaderID);
	
public:

	int terrainShader = 0;
	int renderShader = 0;
	int mousePickingShader = 0;

	bool updateTFB = false;
	
	Transform* transform = nullptr;
	Terrain* terrain = nullptr;
	
};

#endif // !__GAMEOBJECT_H__
