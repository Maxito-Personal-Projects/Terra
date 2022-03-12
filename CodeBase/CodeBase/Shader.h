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
class ShaderResource
{
public:
	
	ShaderResource(std::string Name, ShaderType Type) : Name(Name),Type(Type){}

	bool CompileShaderResource();

public:

	std::string Name = "";
	std::string Code = "";
	ShaderType Type = NONE;
	uint Id = 0;
};


//Shader Program main class
class Shader
{
public:

	Shader(std::string Name) :Name(Name) {}
	~Shader();

	bool CompileShaderProgram();

public:

	std::string Name = "";
	std::vector<ShaderResource*> Shaders;
	uint Id = 0;

};

#endif // !__SHADER_H__
