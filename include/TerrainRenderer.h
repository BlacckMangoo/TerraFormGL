#pragma once 
#include "Shader.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "Light.h"
#include "TerrainWindow.h"

class TerrainRenderer {
public:

    // Constructor (inits shaders/shapes)
    TerrainRenderer(Shader& shader);
    TerrainRenderer() = default;
    ~TerrainRenderer();
    // Renders a defined quad textured with given sprite

    int vertexCount; 

    void Draw(RenderModes mode, const Camera& camera );
    void DrawTerrain(RenderModes mode, const Camera& camera, 
       std::vector<Light*> lights);
    void GenerateTerrain(int width, int height, float scale, float frequency, 
        float amplitude);
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

    void initTerrainRenderData();
    void initTerrainData();
};