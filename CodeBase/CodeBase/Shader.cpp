#include "Shader.h"
#include "Application.h"


Shader::~Shader()
{
	for (int i = 0; i < Shaders.size(); ++i)
	{
		if (Shaders[i])
		{
			delete Shaders[i];
			Shaders[i] = nullptr;
		}
	}
}


bool Shader::CompileShaderProgram()
{
	bool bSuccess = true;

	// Ordering lambda
	auto cmp = [](ShaderResource* A, ShaderResource* B)
	{
		return A->Type > B->Type;
	};

	std::priority_queue<ShaderResource*, std::vector<ShaderResource*>, decltype(cmp)> OrderedShaders(cmp);

	bool bCaptureVars = false;
	for(size_t i = 0; i < Shaders.size(); ++i)
	{
		OrderedShaders.push(Shaders[i]);
		if (Shaders[i]->Type == ShaderType::TESSELLATION_CS ||
			Shaders[i]->Type == ShaderType::TESSELLATION_ES)
		{
			bCaptureVars = true;
		}
	}

	//Generating Shader Program object
	Id = glCreateProgram();

	//Attaching compiled shaders
	while (!OrderedShaders.empty())
	{
		glAttachShader(Id, OrderedShaders.top()->Id);
		OrderedShaders.pop();
	}

	//Telling the program which var/s we want to capture
	if (bCaptureVars)
	{
		const char* CaptureVars[] = { "TFPosition","TFNormal","TFColor" };

		//We want everything in the same buffer!
		glTransformFeedbackVaryings(Id, 3, CaptureVars, GL_INTERLEAVED_ATTRIBS);
	}

	//Compiling Shader Program
	glLinkProgram(Id);

	//Chacking Linking Errors
	int LinkSuccess, ValidProgram;
	char infoLog[512];
	glGetProgramiv(Id, GL_LINK_STATUS, &LinkSuccess);
	glGetProgramiv(Id, GL_VALIDATE_STATUS, &ValidProgram);

	if (!LinkSuccess || !ValidProgram)
	{
		int infoLogLength = 0;
		glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(Id, infoLogLength, NULL, infoLog);
		LOG("Compilation Shader Program Error: %s", infoLog);
		delete[] infoLog;

		bSuccess = false;
	}

	//Deleting Shaders OpenGL
	for (int i = 0; i < Shaders.size(); ++i)
	{
		glDeleteShader(Shaders[i]->Id);
	}

	return bSuccess;
}


// Shader Resource ----------------------------


bool ShaderResource::CompileShaderResource()
{
	bool bSuccess = true;

	//Generating Shader object
	if (Type == VERTEX)
		Id = glCreateShader(GL_VERTEX_SHADER);
	else if (Type == FRAGMENT)
		Id = glCreateShader(GL_FRAGMENT_SHADER);
	else if (Type == TESSELLATION_CS)
		Id = glCreateShader(GL_TESS_CONTROL_SHADER);
	else if (Type == TESSELLATION_ES)
		Id = glCreateShader(GL_TESS_EVALUATION_SHADER);
	else
		Id = glCreateShader(GL_GEOMETRY_SHADER);

	//Attaching the shader source code to the shader object
	const char *shaderCode = Code.c_str();
	glShaderSource(Id, 1, &shaderCode, NULL);

	//Compiling Shader
	glCompileShader(Id);

	//Chacking Compile Errors
	int success;
	char infoLog[512];
	glGetShaderiv(Id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(Id, 512, NULL, infoLog);
		LOG("Compilation Shader Error: %s", infoLog);
		bSuccess = false;
	}

	return bSuccess;
}
