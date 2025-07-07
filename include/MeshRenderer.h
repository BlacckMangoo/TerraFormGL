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


class MeshRenderer {
public:

    // Constructor (inits shaders/shapes)
    MeshRenderer(Shader& shader);
    MeshRenderer() = default;
    ~MeshRenderer();
    // Renders a defined quad textured with given sprite

    int vertexCount; 

    void Draw(RenderModes mode, float dt, const Camera& camera );
    void DrawTerrain(RenderModes mode, float dt, const Camera& camera);
    void GenerateTerrain(int width, int height, float scale, float frequency, 
		float amplitude, float lacunarity);
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

    void initRenderData();
    void initTerrainData();
};