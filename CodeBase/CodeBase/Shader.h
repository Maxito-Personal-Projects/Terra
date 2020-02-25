#ifndef __SHADER_H__
#define __SHADER_H__

#include "Globals.h"
#include <vector>
#include <string>

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	NONE
};

//Shader struct
struct _Shader
{
	_Shader(std::string _name, ShaderType _type)
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

	Shader(std::string _name,_Shader* vertex, _Shader* fragment, _Shader* geometry=nullptr);
	~Shader();

public:

	std::string name = "";
	std::vector<_Shader*> shaders;
	uint id = 0;

};

#endif // !__SHADER_H__
