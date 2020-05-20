#ifndef __MODULESHADER_H__
#define __MODULESHADER_H__

#include "Module.h"

#include <map>

class Shader;
struct _Shader;

class ModuleShader : public Module
{
public:

	ModuleShader(string _name, bool _active=true);
	~ModuleShader();

	//Init Module, Used to create Stuff
	bool Init();

	//Starting Module, Used to init Stuff
	bool Start();

	//PreUpdate Module
	bool PreUpdate();

	//Update Module
	bool Update() { return true; }

	//PreUpdate Module
	bool PosUpdate();

	//Clean Module
	bool CleanUp();

	void GenerateDefaultShaders();
	void GenerateDefaultRenderShaders();
	void GenerateMousePickingShaders();

	bool CompileShader(_Shader* shader);
	bool CompileShaderProgram(Shader* shaderProgram, bool isDefault = false);

	int GetShader(string name);

public:

	map<string, int> shadersNames;
	list<Shader*> shaders;
	
};

#endif // !__MODULESHADER_H__
