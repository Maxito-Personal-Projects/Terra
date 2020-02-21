#include "Shader.h"


Shader::Shader(_Shader* vertex, _Shader* fragment, _Shader* geometry)
{
	shaders.push_back(vertex);
	shaders.push_back(fragment);

	if (geometry)
	{
		shaders.push_back(geometry);
	}

}


Shader::~Shader()
{
}
