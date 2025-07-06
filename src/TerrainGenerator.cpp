#include "TerrainGenerator.h"


std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale)
{
    std::vector<float> terrainData;
    terrainData.reserve((width - 1) * (height - 1) * 6 * 3); // 6 vertices (2 triangles), each 3D

    for (int j = 0; j < height - 1; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            float x0 = i * scale;
            float z0 = j * scale;
            float x1 = (i + 1) * scale;
            float z1 = (j + 1) * scale;

            // Height at each corner
            float y00 = sin(i * 0.2f) * cos(j * 0.2f);
            float y10 = sin((i + 1) * 0.2f) * cos(j * 0.2f);
            float y11 = sin((i + 1) * 0.2f) * cos((j + 1) * 0.2f);
            float y01 = sin(i * 0.2f) * cos((j + 1) * 0.2f);

            // Triangle 1
            terrainData.insert(terrainData.end(), {
                x0, y00, z0,
                x1, y10, z0,
                x1, y11, z1
                });

            // Triangle 2
            terrainData.insert(terrainData.end(), {
                x1, y11, z1,
                x0, y01, z1,
                x0, y00, z0
                });
        }
    }

    return terrainData;
}