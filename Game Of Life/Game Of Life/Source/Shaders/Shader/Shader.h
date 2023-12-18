#pragma once

#include <string>

class Shader
{
public:
	Shader(const char* vertexShaderName, const char* fragmentShaderName);
	unsigned int GetId() const;
	void Activate() const;
	void SetInt(const std::string& name, int value) const;
	void SetBool(const std::string& name, bool value) const;
	void SetFloat(const std::string& name, float value) const;
	void Destroy();

protected:
	Shader() = default;
	unsigned int id = 0;

private:
	unsigned int Compile(const std::string vertexCode, int type, const char* shaderName);
	void Link(unsigned int vertexId, unsigned int fragmentId);
};
