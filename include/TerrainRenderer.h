#pragma once 
#include "Shader.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "Light.h"
enum RenderModes
{
	RENDER_MODE_WIRE_FRAME,
	RENDER_MODE_FILL,
	RENDER_MODE_POINTS

};


class TerrainRenderer {
public:

    // Constructor (inits shaders/shapes)
    TerrainRenderer(Shader& shader);
    TerrainRenderer() = default;
    ~TerrainRenderer();
    // Renders a defined quad textured with given sprite

    int vertexCount; 

    void Draw(RenderModes mode, float dt, const Camera& camera );
    void DrawTerrain(RenderModes mode, float dt, const Camera& camera, 
       std::vector<Light*> lights);
    void GenerateTerrain(int width, int height, float scale, float frequency, 

        float amplitude);
    // Render state
    Shader       shader;
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