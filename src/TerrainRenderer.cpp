#include "TerrainRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "TerrainGenerator.h"
#include <Light.h>
#include "UiManager.h"

TerrainRenderer::TerrainRenderer(Shader& shader)
{
    this->shader = shader;
    this->terrainGenerated = false;
    this->terrainVertexCount = 0;
    this->initTerrainRenderData();
    this->initTerrainData();
}

TerrainRenderer::~TerrainRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    if (terrainGenerated) {
        glDeleteVertexArrays(1, &this->terrainVAO);
        glDeleteBuffers(1, &this->terrainVBO);
    }
}

void TerrainRenderer::initTerrainRenderData()
{
    // Initialize VAO and VBO for potential future use
    // Cube vertices removed - using terrain generation instead
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

void TerrainRenderer::Draw(RenderModes mode ,const  Camera& camera )
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
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

void TerrainRenderer::initTerrainData()
{
    // Initialize terrain VAO and VBO (empty for now)
    glGenVertexArrays(1, &this->terrainVAO);
    glGenBuffers(1, &this->terrainVBO);
    void DrawTerrain(RenderModes mode, const Camera& camera, const std::vector<Light*>& lights);
}

void TerrainRenderer::GenerateTerrain(int width, int height, float scale, float frequency, float amplitude){

    // Generate terrain data using TerrainGenerator
    terrainVertices = terrainGen.generateTerrain(width, height, scale, frequency,
        amplitude);
    terrainVertexCount = terrainVertices.size() / 6; // 6 components per vertex (x, y, z, nx, ny, nz)

    // Bind and upload terrain data to GPU
    glBindVertexArray(terrainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data(), GL_STATIC_DRAW);

    // Position attribute (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Normal attribute (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    terrainGenerated = true;
};


void TerrainRenderer::DrawTerrain(RenderModes mode, const Camera& camera, std::vector<Light*> lights){
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
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 16.0f/9.0f, 0.01f, 2000.0f);
    glm::mat4 mvp = projection * view * model;

    GLuint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    GLuint modelLoc = glGetUniformLocation(shader.ID, "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    const int MAX_LIGHTS = 16; // Must match shader
    int numLights = std::min((int)lights.size(), MAX_LIGHTS);
    std::vector<glm::vec3> lightPositions(numLights);
    std::vector<glm::vec3> lightColors(numLights);
    for (int i = 0; i < numLights; ++i) {
        lightPositions[i] = lights[i]->position;
        lightColors[i] = lights[i]->color;
    }
    shader.SetInteger("numLights", numLights);
    for (int i = 0; i < numLights; ++i) {
        shader.SetVector3f(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
        shader.SetVector3f(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
    }
    shader.SetVector3f("objectColor", glm::vec3(0.5f)); // Mid-gray

    glBindVertexArray(terrainVAO);
    glDrawArrays(GL_TRIANGLES, 0, terrainVertexCount);
    glBindVertexArray(0);

    // Reset polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
