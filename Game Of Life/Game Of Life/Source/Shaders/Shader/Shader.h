#pragma once

#include <string>

using std::string;

class Shader
{
	public:
		Shader(const char* vertexShaderName, const char* fragmentShaderName);
		unsigned int GetId() const;
		void Activate() const;
		void SetInt(const string& name, int value) const;
		void SetBool(const string& name, bool value) const;
		void SetFloat(const string& name, float value) const;
		~Shader();

	protected:
		Shader() = default;
		unsigned int id = 0;

	private:
		unsigned int Compile(const string vertexCode, int type, const char* shaderName);
		void Link(unsigned int vertexId, unsigned int fragmentId);
};
