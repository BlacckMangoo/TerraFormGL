#pragma once 
#include "../Graphics/Shader.h"
#include "../Core/Camera.h"
#include "TerrainGenerator.h"
#include "../Graphics/Light.h"
#include "TerrainWindow.h"
class TerrainRenderer {
public:

    // Constructor (inits shaders/shapes)
    TerrainRenderer(Shader& shader);
    TerrainRenderer() = default;
    ~TerrainRenderer();
    // Renders a defined quad textured with given sprite

    int vertexCount; 

    void DrawTerrain(RenderModes mode, const Camera& camera, 
       std::vector<Light*> lights);
    void GenerateTerrain(int width, int height, float scale, ITerrainHeightStrategy* heightStrategy);
    // Render state
    Shader       shader;
    TerrainWindow terrainWindow;
    unsigned int    VAO;
    unsigned int VBO;
    private:


    // Terrain data
    TerrainGenerator terrainGen;
    std::vector<float> terrainVertices;
    unsigned int terrainVAO;
    unsigned int terrainVBO;
    int terrainVertexCount;
    bool terrainGenerated;
    
    // Terrain bounds in world space
    struct {
        glm::vec3 min;  // Minimum corner of the terrain's bounding box
        glm::vec3 max;  // Maximum corner of the terrain's bounding box
    } bounds;

    void initTerrainRenderData();
    void initTerrainData();

};