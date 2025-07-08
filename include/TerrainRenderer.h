#pragma once 
#include "Shader.h"
#include "Camera.h"
#include "TerrainGenerator.h"

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
        const glm::vec3& lightPos, const glm::vec3& lightColor);
    void GenerateTerrain(int width, int height, float scale, float frequency, 
        float amplitude, float lacunarity, int noiseType, int fractalType, 
        int fractalOctaves, float fractalGain, float fractalWeightedStrength, 
        float fractalPingPongStrength, int cellularDistanceFunction, 
        int cellularReturnType, float cellularJitter, bool useDomainWarp, 
        int domainWarpType, float domainWarpAmp, float domainWarpFreq,
        float time, bool enableDrift);
private:
    // Render state
    Shader       shader;
    unsigned int    VAO;
    unsigned int VBO;

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