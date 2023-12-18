#pragma once

#include <string>

using namespace std;

class ShaderProvider
{
    public:
        static const string GetCode(string shaderName);
#if DEBUG
        static void PackShaders();
#endif

    private:
        static constexpr uint32_t GetHash(const string_view text) noexcept;
#if DEBUG
        static const string ReadFile(const string& shaderName);
        static const string FindFile(const string& searchedFile);
        static void PackShader(const string& shaderPath, ofstream& outputFile);
#else
        static const string UnpackShader(const string& shaderName);
        static const string UnpackCode(const uint32_t& shaderHash);
#endif
};
