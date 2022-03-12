#ifndef __MODULESHADER_H__
#define __MODULESHADER_H__

#include "Module.h"

#include <map>

class Shader;
struct ShaderResource;

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

	void CompileFolderShaders();
	void RecompileAllShaders();
	Shader* CompileShader(string Name);

	int GetShader(string name);

	enum ShaderType FromExtensionToType(const string extension) const;
	string FromTypeToString(const ShaderType Type) const;

public:

	map<string, int> ShadersNames;
	
	list<Shader*> Shaders;

};

#endif // !__MODULESHADER_H__
