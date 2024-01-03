#include "Shader.h"

#include "../ShaderProvider/ShaderProvider.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"
#include "glad/glad.h"

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName)
{
	const std::string vertexCode = ShaderProvider::GetCode(vertexShaderName);
	const std::string fragmentCode = ShaderProvider::GetCode(fragmentShaderName);

	if (!vertexCode.empty() && !fragmentCode.empty())
	{
		unsigned int shaderIds[] = {
			Compile(vertexShaderName, GL_VERTEX_SHADER, vertexCode.c_str()),
			Compile(fragmentShaderName, GL_FRAGMENT_SHADER, fragmentCode.c_str())
		};

		Link(shaderIds, std::size(shaderIds));
	}
}

unsigned int Shader::Compile(const char* shaderName, int type, const char* code)
{
	unsigned int shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &code, NULL);
	glCompileShader(shaderId);

	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shaderId, static_cast<int>(std::size(log)), NULL, log);
		Settings::log << "Shader Error - Compilation failed for \"" <<
			shaderName << "\"\n" << log << "\n";

		return NULL;
	}

	return shaderId;
}

void Shader::Link(unsigned int* shaderIds, size_t shaderCount)
{
	id = glCreateProgram();

	for (unsigned int i = 0 ; i < shaderCount; i++)
		glAttachShader(id, shaderIds[i]);
	
	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(id, static_cast<int>(std::size(log)), NULL, log);
		Settings::log << "Shader Error - Linking failed\n" << log << "\n";
	}

	for (unsigned int i = 0; i < shaderCount; i++)
		glDeleteShader(shaderIds[i]);
}

unsigned int Shader::GetId() const
{
	return id;
}

void Shader::Activate() const
{
	glUseProgram(id);
}

void Shader::SetInt(const string_view name, int value) const
{
	glProgramUniform1i(id, glGetUniformLocation(id, name.data()), value);
}

void Shader::SetBool(const string_view name, bool value) const
{
	SetInt(name, (int)value);
}

void Shader::SetFloat(const string_view name, float value) const
{
	glProgramUniform1f(id, glGetUniformLocation(id, name.data()), value);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}
