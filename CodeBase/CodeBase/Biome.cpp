#include "Application.h"
#include "Biome.h"
#include "GameObject.h"
#include "Chunk.h"

Biome::Biome(std::string _name)
{
	name = _name;

	height = 0.0f;
	frequency = 1.0f;
	seed = 1.0f;

	octaves = 8;
	primitive = 0;
}


Biome::~Biome()
{
}
