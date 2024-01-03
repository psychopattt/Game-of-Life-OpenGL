#pragma once

#include <string>

using std::string_view;

class Shader
{
	public:
		Shader(const char* vertexShaderName, const char* fragmentShaderName);
		unsigned int GetId() const;
		void Activate() const;
		void SetInt(const string_view name, int value) const;
		void SetBool(const string_view name, bool value) const;
		void SetFloat(const string_view name, float value) const;
		~Shader();

	protected:
		Shader() = default;
		unsigned int id = 0;

		unsigned int Compile(const char* shaderName, int type, const char* code);
		void Link(unsigned int* shaderIds, size_t shaderCount);
};
