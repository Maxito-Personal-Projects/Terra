#include "Application.h"
#include "ModuleShader.h"
#include "ModuleInput.h"
#include "FileSystem.h"

#include "Shader.h"



ModuleShader::ModuleShader(string _name, bool _active) : Module(_name,_active)
{
}

ModuleShader::~ModuleShader()
{
	
}

bool ModuleShader::Init()
{
	bool ret = true;
	return ret;
}

bool ModuleShader::Start()
{
	bool ret = true;

	CompileFolderShaders();

	return ret;
}

bool ModuleShader::PreUpdate()
{
	bool ret = true;

	if(myApp->m_input->GetKey(SDL_SCANCODE_F1)==DOWN)
	{
		LOG("Recompiling All Shaders!");
		RecompileAllShaders();
	}

	return ret;
}

bool ModuleShader::PosUpdate()
{
	bool ret = true;
	return ret;
}

bool ModuleShader::CleanUp()
{
	bool ret = true;

	for (list<Shader*>::iterator s_it = Shaders.begin(); s_it != Shaders.end(); ++s_it)
	{
		if ((*s_it) != nullptr)
		{
			delete (*s_it);
			(*s_it) = nullptr;
		}
	}

	Shaders.clear();
	ShadersNames.clear();

	return ret;
}

void ModuleShader::CompileFolderShaders()
{
	vector<string> DefaultShaders;
	vector<string> CompiledShaders;

	myApp->fileSystem->GetAllFilesInDirectory("Shaders", DefaultShaders, true);
	
	for (size_t i = 0; i < DefaultShaders.size(); ++i)
	{
		// Cheking if the shader is already compiled
		if (find(begin(CompiledShaders), end(CompiledShaders), DefaultShaders[i]) != end(CompiledShaders))
		{
			continue;
		}

		CompiledShaders.push_back(DefaultShaders[i]);

		if (Shader* NewShader = CompileShader(DefaultShaders[i]))
		{
			Shaders.push_back(NewShader);
			ShadersNames.insert({ NewShader->Name, NewShader->Id });
		}
	}
}

enum ShaderType ModuleShader::FromExtensionToType(const string extension) const
{
	ShaderType Type = ShaderType::NONE;

	if (extension == "vrt")
	{
		Type = ShaderType::VERTEX;
	}
	else if (extension == "frg")
	{
		Type = ShaderType::FRAGMENT;
	}
	else if (extension == "ctrl")
	{
		Type = ShaderType::TESSELLATION_CS;
	}
	else if (extension == "eval")
	{
		Type = ShaderType::TESSELLATION_ES;
	}
	else
	{
		Type = ShaderType::GEOMETRY;
	}

	return Type;
}

string ModuleShader::FromTypeToString(const ShaderType Type) const
{
	switch (Type)
	{
	case ShaderType::FRAGMENT:
		return "Fragment Shader";
	case ShaderType::VERTEX:
		return "Vertex Shader";
	case ShaderType::GEOMETRY:
		return "Geometry Shader";
	case ShaderType::TESSELLATION_CS:
		return "Tessellation Control Shader";
	case ShaderType::TESSELLATION_ES:
		return "Tessellation Evaluation Shader";
	case ShaderType::NONE:
		return "None Type Shader";
	default:
		return "Unknown Shader";
	}
}

// Shader order: Vertex, Geometry, Control, Evaluation, Fragment

Shader* ModuleShader::CompileShader(string Name)
{
	Shader* ShaderProgram = new Shader(Name);
	
	vector<string> AllShaders;
	myApp->fileSystem->GetAllFilesInDirectory("Shaders", AllShaders);

	//Compile shader Resources
	for (size_t i = 0; i < AllShaders.size(); ++i)
	{
		string FileName = myApp->fileSystem->RemoveExtension(AllShaders[i]);
		if (FileName == Name)
		{
			ShaderType Type = FromExtensionToType(myApp->fileSystem->GetFileExtension(AllShaders[i]));
			ShaderResource* ShaderRes = new ShaderResource(Name, Type);
			ShaderRes->Code = myApp->fileSystem->FileToString("Shaders/" + AllShaders[i]);

			if (ShaderRes->CompileShaderResource())
			{
				LOG("%s - %s Compiled Successfully", Name.c_str(), FromTypeToString(Type).c_str());
				ShaderProgram->Shaders.push_back(ShaderRes);
			}
			else
			{
				LOG("Something went wrong compiling %s program", Name.c_str());
				delete ShaderProgram;
				ShaderProgram = nullptr;
				break;
			}
		}
	}

	//Compile Shader Program
	if (ShaderProgram)
	{
		if (ShaderProgram->CompileShaderProgram())
		{
			LOG("%s Program Compiled Successfully", Name.c_str());
		}
		else
		{
			LOG("Error compiling %s program", Name.c_str());
			delete ShaderProgram;
			ShaderProgram = nullptr;
		}
	}

	return ShaderProgram;
}


int ModuleShader::GetShader(string name)
{
	if (ShadersNames.find(name) == ShadersNames.end())
	{
		LOG("Culdn't find shader %s", name.c_str());
	}

	return ShadersNames[name];
}


void ModuleShader::RecompileAllShaders()
{
	for (Shader* CurrentShader : Shaders)
	{
		int OldId = CurrentShader->Id;
		bool Recompile = true;

		for(ShaderResource* CurrentShaderResource : CurrentShader->Shaders)
		{
			if (!CurrentShaderResource->CompileShaderResource())
			{
				LOG("Error Recompiling %s program, using old version", CurrentShader->Name.c_str());
				Recompile = false;
				break;
			}
		}

		if(Recompile)
		{
			glDeleteProgram(OldId);
			
			if (CurrentShader->CompileShaderProgram())
			{
				ShadersNames[CurrentShader->Name] = CurrentShader->Id;
			}
			else
			{
				LOG("Error Recompiling %s program", CurrentShader->Name.c_str());
			}
		}
	}
}