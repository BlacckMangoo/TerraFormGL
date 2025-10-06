#pragma once

#include <map>
#include <string>

#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"


\
class ResourceManager
{
public:
   
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    static Shader&   GetShader(std::string name);
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    static Shader LoadComputeShader(const char* cShaderFile, std::string name);

    static Texture2D GetTexture(std::string name);
    static void      Clear();
private:
    ResourceManager() {}
 
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
   
    static Texture2D loadTextureFromFile(const char* file, bool alpha);

    static Shader loadComputeShaderFromFile(const char* cShaderFile);


};