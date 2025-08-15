#pragma once 
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

using namespace std;
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 TexCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int texId; 
	string type; 
	string path; 
};