#include "Application.h"
#include "ModuleShader.h"
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

	CompileAllShaders();
	
	return ret;
}

bool ModuleShader::PreUpdate()
{
	bool ret = true;
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

	for (list<Shader*>::iterator s_it = shaders.begin(); s_it != shaders.end(); ++s_it)
	{
		if ((*s_it) != nullptr)
		{
			delete (*s_it);
			(*s_it) = nullptr;
		}
	}

	shaders.clear();
	shadersNames.clear();

	return ret;
}

void ModuleShader::CompileAllShaders()
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
			shaders.push_back(NewShader);
			shadersNames.insert({ NewShader->name, NewShader->id });
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
	Shader* ShaderProgram = new Shader();
	ShaderProgram->name = Name;
	
	vector<string> AllShaders;
	myApp->fileSystem->GetAllFilesInDirectory("Shaders", AllShaders);

	// Ordering lambda
	auto cmp = [](ShaderResource* A, ShaderResource* B)
	{
		return A->type > B->type;
	};

	std::priority_queue<ShaderResource*, std::vector<ShaderResource*>, decltype(cmp)> OrderedShaders(cmp);

	//Compile shader Resources
	for (size_t i = 0; i < AllShaders.size(); ++i)
	{
		string FileName = myApp->fileSystem->RemoveExtension(AllShaders[i]);
		if (FileName == Name)
		{
			ShaderType Type = FromExtensionToType(myApp->fileSystem->GetFileExtension(AllShaders[i]));
			ShaderResource* ShaderRes = new ShaderResource(Name, Type);
			ShaderRes->code = myApp->fileSystem->FileToString("Shaders/" + AllShaders[i]);

			if (CompileShaderResource(ShaderRes))
			{
				LOG("%s - %s Compiled Successfully", Name.c_str(), FromTypeToString(Type).c_str());
				OrderedShaders.push(ShaderRes);
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
		bool CaptureVars = false;

		//Fill shader vector in order
		while(!OrderedShaders.empty())
		{
			ShaderProgram->shaders.push_back(OrderedShaders.top());
			OrderedShaders.pop();

			if(ShaderProgram->shaders.back()->type == ShaderType::TESSELLATION_CS ||
				ShaderProgram->shaders.back()->type == ShaderType::TESSELLATION_ES)
			{
				CaptureVars = true;
			}
		}

		if (CompileShaderProgram(ShaderProgram, CaptureVars))
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


bool ModuleShader::CompileShaderResource(ShaderResource* shader)
{
	bool ret = true;

	//Generating Shader object
	if(shader->type== VERTEX)
		shader->id = glCreateShader(GL_VERTEX_SHADER);
	else if(shader->type==FRAGMENT)
		shader->id = glCreateShader(GL_FRAGMENT_SHADER);
	else if (shader->type == TESSELLATION_CS)
		shader->id = glCreateShader(GL_TESS_CONTROL_SHADER);
	else if (shader->type == TESSELLATION_ES)
		shader->id = glCreateShader(GL_TESS_EVALUATION_SHADER);
	else
		shader->id = glCreateShader(GL_GEOMETRY_SHADER);

	//Attaching the shader source code to the shader object
	const char *shaderCode = shader->code.c_str();
	glShaderSource(shader->id, 1, &shaderCode, NULL);

	//Compiling Shader
	glCompileShader(shader->id);

	//Chacking Compile Errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader->id, 512, NULL, infoLog);
		LOG("Compilation Shader Error: %s", infoLog);
		ret = false;
	}

	return ret;
}

bool ModuleShader::CompileShaderProgram(Shader* shaderProgram, bool isDefault)
{
	bool ret = true;

	//Generating Shader Program object
	shaderProgram->id = glCreateProgram();

	//Attaching compiled shaders
	for (int i = 0; i < shaderProgram->shaders.size(); ++i)
	{
		glAttachShader(shaderProgram->id, shaderProgram->shaders[i]->id);
	}

	//Telling the program which var/s we want to capture
	if (isDefault)
	{
		const char* captureVars[] = { "TFPosition","TFNormal","TFColor" };
	
		//We want everything in the same buffer!
		glTransformFeedbackVaryings(shaderProgram->id, 3, captureVars, GL_INTERLEAVED_ATTRIBS);
	}

	//Compiling Shader Program
	glLinkProgram(shaderProgram->id);

	//Chacking Linking Errors
	int LinkSuccess, ValidProgram;
	char infoLog[512];
	glGetProgramiv(shaderProgram->id, GL_LINK_STATUS, &LinkSuccess);
	glGetProgramiv(shaderProgram->id, GL_VALIDATE_STATUS, &ValidProgram);

	if (!LinkSuccess || !ValidProgram)
	{
		int infoLogLength = 0;
		glGetProgramiv(shaderProgram->id, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shaderProgram->id, infoLogLength, NULL, infoLog);
		LOG("Compilation Shader Program Error: %s", infoLog);
		delete[] infoLog;

		ret = false;
	}

	//Deleting Shaders
	for (int i = 0; i < shaderProgram->shaders.size(); ++i)
	{
		glDeleteShader(shaderProgram->shaders[i]->id);
	}

	return ret;
}

int ModuleShader::GetShader(string name)
{
	if (shadersNames.find(name) == shadersNames.end())
	{
		LOG("Culdn't find shader %s", name.c_str());
	}

	return shadersNames[name];
}
