#ifndef __BIOME_H__
#define __BIOME_H__

#include <string>
#include <list>

class GameObject;
class Chunk;

class Biome
{
public:

	Biome(std::string _name);
	~Biome();

public:

	GameObject* parent = nullptr;
	std::list<Chunk*> chunks;

	float height = 0.0f;
	float frequency = 1.0f;
	float seed = 1.0f;

	int octaves = 8;
	int primitive = 0;

	std::string name = "";
};

#endif // !__BIOME_H__
