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
	std::string code = "";
	ShaderType type = NONE;
	uint shaderId = 0;
};

//Shader Program main class
class Shader
{
public:

	Shader(_Shader* vertex, _Shader* fragment, _Shader* geometry=nullptr);
	~Shader();

public:

	std::vector<_Shader*> shaders;
	uint shaderProgramId = 0;

};

#endif // !__SHADER_H__
