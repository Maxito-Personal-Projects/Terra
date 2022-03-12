#include "Shader.h"


Shader::Shader()
{

}


Shader::~Shader()
{
	for (int i = 0; i < shaders.size(); ++i)
	{
		if (shaders[i])
		{
			delete shaders[i];
			shaders[i] = nullptr;
		}
	}
}
