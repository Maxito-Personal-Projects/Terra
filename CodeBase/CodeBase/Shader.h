#ifndef __SHADER_H__
#define __SHADER_H__

#include "Globals.h"
#include <vector>
#include <string>
#include <queue>

enum ShaderType
{
	VERTEX = 0,
	GEOMETRY = 1,
	TESSELLATION_CS = 2,
	TESSELLATION_ES = 3,
	FRAGMENT = 4,
	NONE = 255
};

//Shader struct
struct ShaderResource
{
	ShaderResource(std::string _name, ShaderType _type)
	{
		name = _name;
		type = _type;
	}

	std::string name = "";
	std::string code = "";
	ShaderType type = NONE;
	uint id = 0;
};

//Shader Program main class
class Shader
{
public:

	Shader();
	~Shader();

public:

	std::string name = "";
	std::vector<ShaderResource*> shaders;
	uint id = 0;

};

#endif // !__SHADER_H__
