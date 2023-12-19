#include "ShaderProvider.h"

#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"

#if DEBUG
#include <fstream>
#include <sstream>
#include <filesystem>
#endif

static const string shaderExtension = ".glsl";

const string ShaderProvider::GetCode(string shaderName)
{
    if (!shaderName.ends_with(shaderExtension))
        shaderName += shaderExtension;

#if DEBUG
    return ReadFile(shaderName);
#else
    return UnpackShader(shaderName);
#endif
}

constexpr uint32_t ShaderProvider::GetHash(const string_view text) noexcept
{
    uint32_t hash = 5381;

    for (const auto& c : text)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

#if DEBUG

static const string shaderPath = "../../Game Of Life/Source/Shaders/Shaders/";
static const string packedShaderPath = "../../Game Of Life/Source/Shaders/ShaderProvider/PackedShaders.cpp";

const string ShaderProvider::ReadFile(const string& shaderName)
{
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    const string& fullPath = FindFile(shaderName);

    try
    {
        file.open(fullPath, ios_base::in);
        stringstream stream;
        stream << file.rdbuf();
        file.close();

        return stream.str();
    }
    catch (ifstream::failure exception)
    {
        Settings::log << "Shader Error - Failed to load file \"" << fullPath << "\"\n";
        return "";
    }
}

const string ShaderProvider::FindFile(const string& searchedFile)
{
    using namespace filesystem;

    if (exists(searchedFile))
        return searchedFile;

    for (const directory_entry& file : recursive_directory_iterator(shaderPath))
    {
        const string& filePath = file.path().string();

        if (filePath.ends_with(searchedFile))
            return filePath;
    }

    Settings::log << "Shader Error - Failed to find file \"" << searchedFile << "\"\n";
    return searchedFile;
}

void ShaderProvider::PackShaders()
{
    using namespace filesystem;

    ofstream outputFile;
    outputFile.open(packedShaderPath, ios_base::out | ios_base::trunc);

    outputFile << "// Generated code\n\n#if !DEBUG\n#include \""
        "ShaderProvider.h\"\n#include <string>\nconst string "
        "ShaderProvider::UnpackCode(const uint32_t& h) {\n"
        "switch (h) {\n";

    for (const directory_entry& file : recursive_directory_iterator(shaderPath))
    {
        const path& filePath = file.path();

        if (file.is_regular_file() && filePath.extension() == shaderExtension)
            PackShader(filePath.string(), outputFile);
    }

    outputFile << "default: return \"\"; }}\n#endif\n";
    
    outputFile.close();
    Settings::log << "Shader Packing - Finished packing shaders\n";
}

void ShaderProvider::PackShader(const string& shaderPath, ofstream& outputFile)
{
    string code = ReadFile(shaderPath);
    const string shaderName = filesystem::path(shaderPath).filename().string();

    if (code.empty())
    {
        Settings::log << "Shader Packing - Failed to pack shader \"" << shaderName << "\"\n";
        return;
    }

    size_t matchPosition = code.find("\n");

    while (matchPosition != string::npos)
    {
        code.replace(matchPosition, 1, "\\n");
        matchPosition = code.find("\n", matchPosition + 1);
    }

    outputFile << "case " << GetHash(shaderName) << ": return \"" << code << "\";\n";
    Settings::log << "Shader Packing - Successfully packed \"" << shaderName << "\"\n";
}

#else

const string ShaderProvider::UnpackShader(const string& shaderName)
{
    const string& code = UnpackCode(GetHash(shaderName));

    if (code.empty())
        Settings::log << "Shader Error - Failed to load shader \"" << shaderName << "\"\n";

    return code;
}

#endif
