#include "../../include/Terrain/TerrainRenderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../include/Core/Camera.h"
#include "../../include/Terrain/TerrainGenerator.h"
#include "../../include/Graphics/Light.h"
#include <iostream>

TerrainRenderer::TerrainRenderer(Shader& shader, Shader& computeShader)
{
    this->shader = shader;
    this->computeShader = computeShader;
    this->terrainGenerated = false;
    this->terrainVertexCount = 0;
    this->initTerrainData();
}

TerrainRenderer::~TerrainRenderer()
{

    if (terrainGenerated) {
        glDeleteVertexArrays(1, &this->terrainVAO);
        glDeleteBuffers(1, &this->terrainSSBO);
    }
}



void TerrainRenderer::initTerrainData()
{
    glGenVertexArrays(1, &this->terrainVAO);
    glGenBuffers(1, &this->terrainSSBO);
}

void TerrainRenderer::GenerateTerrain(int width, int height, float scale,  ITerrainHeightStrategy* heightStrategy){
    terrainVertices = terrainGen.generateTerrain(width, height, scale,heightStrategy);
    terrainVertexCount = terrainVertices.size() / 6;
    glBindVertexArray(terrainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainSSBO);
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data() ,GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0, terrainSSBO);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    terrainGenerated = true;
    
}

void TerrainRenderer::GenerateTerrain(int width, int height, float scale){
    terrainVertices = terrainGen.generateTerrain(width, height, scale);

   terrainVertexCount = (GLsizei)terrainVertices.size() / 6;
    glBindVertexArray(terrainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainSSBO);
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data() ,GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, terrainSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, terrainSSBO);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    terrainGenerated = true;
    

}

void TerrainRenderer::DrawTerrain(RenderModes mode, const Camera& camera, std::vector<Light*> lights){
    if (!terrainGenerated) {
        return; // No terrain to render
    }
    computeShader.Use();
    computeShader.SetFloat("deltaTime", (float)glfwGetTime());

 
    const GLuint LOCAL_SIZE_X = 128u; // example; must match compute shader
    GLuint numVertices = (GLuint)terrainVertexCount; // this is vertex count
    GLuint groups = (numVertices + LOCAL_SIZE_X - 1) / LOCAL_SIZE_X;


    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, terrainSSBO);

    glDispatchCompute(groups, 1, 1);

    // Ensure writes are visible to vertex fetch
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);


    // Set polygon mode based on selected render mode
    if (mode == RENDER_MODE_WIRE_FRAME) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (mode == RENDER_MODE_FILL) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (mode == RENDER_MODE_POINTS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    shader.Use();

    // Matrices
    glm::mat4 model = glm::mat4(1.0f); 
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 mvp = projection * view * model;

    shader.SetFloat("time", glfwGetTime());



    GLuint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");
    if (mvpLoc != -1) {
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    } 

    GLuint modelLoc = glGetUniformLocation(shader.ID, "u_Model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    const int MAX_LIGHTS = 16; 
    int numLights = std::min((int)lights.size(), MAX_LIGHTS);
    std::vector<glm::vec3> lightPositions(numLights);
    std::vector<glm::vec3> lightColors(numLights);
    for (int i = 0; i < numLights; ++i) {
        lightPositions[i] = lights[i]->position;
        lightColors[i] = lights[i]->color;
    }
    
    GLuint numLightsLoc = glGetUniformLocation(shader.ID, "numLights");
    if (numLightsLoc != -1) {
        shader.SetInteger("numLights", numLights);
    } 
    
    for (int i = 0; i < numLights; ++i) {
        std::string posName = "lightPositions[" + std::to_string(i) + "]";
        std::string colorName = "lightColors[" + std::to_string(i) + "]";
        
        GLuint posLoc = glGetUniformLocation(shader.ID, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shader.ID, colorName.c_str());
        
        if (posLoc != -1) {
            shader.SetVector3f(posName.c_str(), lightPositions[i]);
        }
        if (colorLoc != -1) {
            shader.SetVector3f(colorName.c_str(), lightColors[i]);
        }
    }
    
    GLuint objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
    if (objectColorLoc != -1) {
        shader.SetVector3f("objectColor", glm::vec3(0.5f)); 
    }


    if (terrainVAO != 0 && terrainVertexCount > 0) {
        glBindVertexArray(terrainVAO);
        glDrawArrays(GL_TRIANGLES, 0, terrainVertexCount);
        glBindVertexArray(0);
    } 

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

