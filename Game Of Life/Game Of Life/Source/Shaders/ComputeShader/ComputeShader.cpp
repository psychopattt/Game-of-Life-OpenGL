#include "ComputeShader.h"

#include <string>

#include "../ShaderProvider/ShaderProvider.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"
#include "glad/glad.h"

ComputeShader::ComputeShader(const char* shaderName, unsigned int width,
	unsigned int height, unsigned int depth)
{
	const std::string code = ShaderProvider::GetCode(shaderName);

	if (!code.empty())
	{
		unsigned int shaderId = Compile(shaderName, GL_COMPUTE_SHADER, code.c_str());
		Link(&shaderId, 1);
		ComputeGroupCounts(width, height, depth);
	}
}

void ComputeShader::ComputeGroupCounts(unsigned int globalWidth,
	unsigned int globalHeight, unsigned int globalDepth)
{
	int localSize[] = { 1, 1, 1 };
	glGetProgramiv(id, GL_COMPUTE_WORK_GROUP_SIZE, localSize);
	unsigned int globalSize[] = { globalWidth, globalHeight, globalDepth };

	for (char i = 0; i < std::size(groupCounts); i++)
		groupCounts[i] = globalSize[i] / localSize[i];
}

void ComputeShader::Execute() const
{
	Activate();
	glDispatchCompute(groupCounts[0], groupCounts[1], groupCounts[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
