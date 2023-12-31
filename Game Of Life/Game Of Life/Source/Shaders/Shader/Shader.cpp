#include "Shader.h"

#include "../ShaderProvider/ShaderProvider.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"
#include "glad/glad.h"

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName)
{
	const string vertexCode = ShaderProvider::GetCode(vertexShaderName);
	const string fragmentCode = ShaderProvider::GetCode(fragmentShaderName);

	if (!vertexCode.empty() && !fragmentCode.empty())
	{
		unsigned int vertexId = Compile(vertexCode, GL_VERTEX_SHADER, vertexShaderName);
		unsigned int fragmentId = Compile(fragmentCode, GL_FRAGMENT_SHADER, fragmentShaderName);
		Link(vertexId, fragmentId);
	}
}

unsigned int Shader::Compile(const string code, int type, const char* shaderName)
{
	int success;
	char log[512];
	const char* codeChars = code.c_str();

	unsigned int shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &codeChars, NULL);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderId, sizeof(log) / sizeof(*log), NULL, log);
		Settings::log << "Shader Error - Compilation failed for \"" << shaderName << "\"\n" << log << "\n";
		return NULL;
	}

	return shaderId;
}

void Shader::Link(unsigned int vertexId, unsigned int fragmentId)
{
	int success;
	char log[512];

	id = glCreateProgram();
	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(log) / sizeof(*log), NULL, log);
		Settings::log << "Shader Error - Linking failed\n" << log << "\n";
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

unsigned int Shader::GetId() const
{
	return id;
}

void Shader::Activate() const
{
	glUseProgram(id);
}

void Shader::SetInt(const string& name, int value) const
{
	glProgramUniform1i(id, glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetBool(const string& name, bool value) const
{
	SetInt(name, (int)value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glProgramUniform1f(id, glGetUniformLocation(id, name.c_str()), value);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}
