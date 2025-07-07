#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "TerrainGenerator.h"



MeshRenderer::MeshRenderer(Shader& shader)
{
    this->shader = shader;
    this->terrainGenerated = false;
    this->terrainVertexCount = 0;
    this->initRenderData();
    this->initTerrainData();
}

MeshRenderer::~MeshRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    if (terrainGenerated) {
        glDeleteVertexArrays(1, &this->terrainVAO);
        glDeleteBuffers(1, &this->terrainVBO);
    }
}

void MeshRenderer::initRenderData()
{
    // Initialize VAO and VBO for potential future use
    // Cube vertices removed - using terrain generation instead
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

void MeshRenderer::Draw(RenderModes mode, float dt ,const  Camera& camera )
{
    // Set polygon mode
    if (mode == RENDER_MODE_WIRE_FRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (mode == RENDER_MODE_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (mode == RENDER_MODE_POINTS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    shader.Use();

    // Matrices
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), dt, glm::vec3(1.0f, 1.0f, 0.0f));// Rotate around Z-axis
    glm::mat4 view =  glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp); // Camera view matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); // You can pass aspect

    glm::mat4 mvp = projection * view * model;

    // Upload to shader
    GLuint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void MeshRenderer::initTerrainData()
{
    // Initialize terrain VAO and VBO (empty for now)
    glGenVertexArrays(1, &this->terrainVAO);
    glGenBuffers(1, &this->terrainVBO);
}

void MeshRenderer::GenerateTerrain(int width, int height, float scale, float frequency,
	float amplitude, int seed, int noiseType, int fractalType, 
	int octaves, float lacunarity, float gain)
{
    // Generate terrain data using TerrainGenerator
    terrainVertices = terrainGen.generateTerrain(width, height, scale, frequency, 
		amplitude, seed, noiseType, fractalType, octaves, lacunarity, gain);
    terrainVertexCount = terrainVertices.size() / 3; // 3 components per vertex (x, y, z)

    // Bind and upload terrain data to GPU
    glBindVertexArray(terrainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data(), GL_STATIC_DRAW);

    // Position attribute (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    terrainGenerated = true;
}

void MeshRenderer::DrawTerrain(RenderModes mode, float dt, const Camera& camera)
{
    if (!terrainGenerated) {
        return; // No terrain to render
    }

    // Set polygon mode
    if (mode == RENDER_MODE_WIRE_FRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (mode == RENDER_MODE_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (mode == RENDER_MODE_POINTS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    shader.Use();

    // Matrices
    glm::mat4 model = glm::mat4(1.0f); // Keep terrain static, no rotation
    glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 1.0f, 0.1f, 100.0f);

    glm::mat4 mvp = projection * view * model;

    // Upload to shader
    GLuint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(terrainVAO);
    glDrawArrays(GL_TRIANGLES, 0, terrainVertexCount);
    glBindVertexArray(0);
}
