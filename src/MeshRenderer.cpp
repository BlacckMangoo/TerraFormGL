#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <vector>
#include "TerrainGenerator.h"
#include "iostream"
MeshRenderer::MeshRenderer(Shader& shader)
{
    this->shader = shader;
    this->initRenderData();
}

MeshRenderer::~MeshRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void MeshRenderer::initRenderData()
{
	TerrainGenerator terrainGenerator; 
	std::vector<float> vertices = terrainGenerator.generateTerrain(100, 100, 1.0f);
	this->vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshRenderer::Draw(RenderModes mode ,const  Camera& camera )
{
    // Set polygon mode
    if (mode == RENDER_MODE_WIRE_FRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (mode == RENDER_MODE_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (mode == RENDER_MODE_POINTS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    shader.Use();

  
    glm::mat4 view = camera.view; // Use the camera's view matrix
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 16.0f/9.0f, 0.1f, 100.0f); // Proper aspect ratio

    glm::mat4 mvp = projection * view ;

    // Upload to shader
    GLuint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
