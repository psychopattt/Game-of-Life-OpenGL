#include "ComputeShader.h"

#include <iostream>

#include "glad/glad.h"
#include "../ShaderProvider/ShaderProvider.h"

using namespace std;

ComputeShader::ComputeShader(const char* shaderName, unsigned int width, unsigned int height, unsigned int depth)
{
	const string code = ShaderProvider::GetCode(shaderName);

	if (!code.empty())
	{
		unsigned int globalSize[3] = { width, height, depth };
		copy(globalSize, globalSize + 3, this->globalSize);

		const unsigned int shaderId = Compile(code, shaderName);
		Link(shaderId, shaderName);
		ExtractLocalGroupSize(code);
	}
}

unsigned int ComputeShader::Compile(const string code, const char* shaderName)
{
	int success;
	char log[512];
	const char* codeChars = code.c_str();

	unsigned int shaderId = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderId, 1, &codeChars, NULL);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderId, sizeof(log) / sizeof(*log), NULL, log);
		cout << "Compute Shader Error - Compilation failed for \"" << shaderName << "\"\n" << log << endl;
		return NULL;
	}

	return shaderId;
}

void ComputeShader::Link(unsigned int shaderId, const char* shaderName)
{
	int success;
	char log[512];

	id = glCreateProgram();
	glAttachShader(id, shaderId);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(log) / sizeof(*log), NULL, log);
		cout << "Compute Shader Error - Linking failed for \"" << shaderName << "\"\n" << log << endl;
	}

	glDeleteShader(shaderId);
}

void ComputeShader::ExtractLocalGroupSize(const string code)
{
	glGetProgramiv(id, GL_COMPUTE_WORK_GROUP_SIZE, localSize);
}

void ComputeShader::Execute() const
{
	Activate();

	glDispatchCompute(
		globalSize[0] / localSize[0],
		globalSize[1] / localSize[1],
		globalSize[2] / localSize[2]
	);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
