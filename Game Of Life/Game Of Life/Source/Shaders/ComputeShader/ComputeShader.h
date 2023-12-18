#pragma once

#include "../Shader/Shader.h"

class ComputeShader : public Shader
{
	public:
		ComputeShader(const char* shaderName, unsigned int width, unsigned int height, unsigned int depth = 1);
		void Execute() const;

	private:
		int localSize[3] = { 1, 1, 1 };
		unsigned int globalSize[3] = { };

		void ExtractLocalGroupSize(const std::string code);
		unsigned int Compile(const std::string code, const char* shaderName);
		void Link(unsigned int shaderId, const char* shaderName);
};
