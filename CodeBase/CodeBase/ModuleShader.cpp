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

	GenerateDefaultShaders();

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

void ModuleShader::GenerateDefaultShaders()
{
	//Creating default Vertex Shader
	_Shader* defaultVertexSahder = new _Shader("Default Vertex Shader", VERTEX);
	defaultVertexSahder->code = myApp->fileSystem->FileToString("Shaders/Default_Vertex_Shader.txt");
	
	if (CompileShader(defaultVertexSahder))
	{
		LOG("Default Vertex Shader Compiled Successfully");
	}

	//Creating default Geometry Shader
	_Shader* defaultGeometrySahder = new _Shader("Default Geometry Shader", GEOMETRY);
	defaultGeometrySahder->code = myApp->fileSystem->FileToString("Shaders/Default_Geometry_Shader.txt");

	if (CompileShader(defaultGeometrySahder))
	{
		LOG("Default Geometry Shader Compiled Successfully");
	}

	//Creating default Tessellation Control Shader
	_Shader* defaultTCSahder = new _Shader("Default Tessellation Control Shader", TESSELLATION_CS);
	defaultTCSahder->code = myApp->fileSystem->FileToString("Shaders/Default_Control_Shader.txt");

	if (CompileShader(defaultTCSahder))
	{
		LOG("Default Tessellation Control Shader Compiled Successfully");
	}

	//Creating default Tessellation Evaluation Shader
	_Shader* defaultTESahder = new _Shader("Default Tessellation Evaluation Shader", TESSELLATION_ES);
	defaultTESahder->code = myApp->fileSystem->FileToString("Shaders/Default_Evaluation_Shader.txt");

	if (CompileShader(defaultTESahder))
	{
		LOG("Default Tessellation Evaluation Shader Compiled Successfully");
	}

	//Creating default Fragment Shader
	_Shader* defaultFragmentSahder = new _Shader("Default Fragment Shader", FRAGMENT);
	defaultFragmentSahder->code = myApp->fileSystem->FileToString("Shaders/Default_Fragment_Shader.txt");
	
	if(CompileShader(defaultFragmentSahder))
	{
		LOG("Default Fragment Shader Compiled Successfully");
	}

	////Creating Default Shader Program
	//Shader* defaultShader = new Shader("Default Shader", defaultVertexSahder, defaultFragmentSahder, defaultGeometrySahder);
	//if (CompileShaderProgram(defaultShader))
	//{
	//	LOG("Default Shader Program Compiled Successfully");

	//	shaders.push_back(defaultShader);
	//	shadersNames.insert({ defaultShader->name, defaultShader->id });
	//}

	//Creating Default Shader Program
	Shader* defaultShader = new Shader("Default Shader", defaultVertexSahder, defaultFragmentSahder, nullptr, defaultTCSahder,defaultTESahder);
	if (CompileShaderProgram(defaultShader))
	{
		LOG("Default Shader Program Compiled Successfully");

		shaders.push_back(defaultShader);
		shadersNames.insert({ defaultShader->name, defaultShader->id });
	}

}

bool ModuleShader::CompileShader(_Shader* shader)
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

bool ModuleShader::CompileShaderProgram(Shader * shaderProgram)
{
	bool ret = true;

	//Generating Shader Program object
	shaderProgram->id = glCreateProgram();

	//Attaching compiled shaders
	for (int i = 0; i < shaderProgram->shaders.size(); ++i)
	{
		glAttachShader(shaderProgram->id, shaderProgram->shaders[i]->id);
	}

	//Compiling Shader Program
	glLinkProgram(shaderProgram->id);

	//Chacking Compile Errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram->id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderProgram->id, 512, NULL, infoLog);
		LOG("Compilation Shader Program Error: %s", infoLog);
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
	return shadersNames[name];
}
