#ifndef __TERRAIN_H__
#define __TERRAIN_H__

class GameObject;

class Terrain
{
public:
	Terrain(GameObject* _parent);
	~Terrain();

public:

	GameObject* parent = nullptr;

	float maxHeight = 20.0f;
	float seed = 1.0f;
	float delta = 0.01f;
	float frequency = 1.0f;
	int octaves = 8;

	bool heightmap = false;
	bool perlin = false;
	bool voronoi = false;
	bool brownian = true;

};

#endif // !__TERRAIN_H__
