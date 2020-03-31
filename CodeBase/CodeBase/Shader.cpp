#include "Shader.h"


Shader::Shader(std::string _name, _Shader* vertex, _Shader* fragment, _Shader* geometry, _Shader* tessControl, _Shader* tessEvaluation)
{
	name = _name;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	if (geometry)
	{
		shaders.push_back(geometry);
	}
	if (tessControl && tessEvaluation)
	{
		shaders.push_back(tessControl);
		shaders.push_back(tessEvaluation);
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
