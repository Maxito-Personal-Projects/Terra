#include "Shader.h"


Shader::Shader(std::string _name, _Shader* vertex, _Shader* fragment, _Shader* geometry)
{
	name = _name;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	if (geometry)
	{
		shaders.push_back(geometry);
	}
}


Shader::~Shader()
{
	for (int i = 0; i < shaders.size(); ++i)
	{
		if (shaders[i] != nullptr)
		{
			delete shaders[i];
			shaders[i] = nullptr;
		}
	}
}
