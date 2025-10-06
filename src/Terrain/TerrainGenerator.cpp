#include <Terrain/TerrainGenerator.h>
#include "../../include/Terrain/TerrainStrategy.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale,ITerrainHeightStrategy* heightStrategy)
{
	
    std::vector<float> terrainData;

    terrainData.reserve((width - 1) * (height - 1) * 6 * 6);

    for (int j = 0; j < height - 1; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            float x0 = i * scale;
            float z0 = j * scale;
            float x1 = (i + 1) * scale;
            float z1 = (j + 1) * scale;

            float y00 = heightStrategy->GetHeight(x0,z0);
            float y10 = heightStrategy->GetHeight(x1,z0);
            float y11 = heightStrategy->GetHeight(x1,z1);
            float y01 = heightStrategy->GetHeight(x0,z1);

            
     
            // Create vertices for the quad
            glm::vec3 v0(x0, y00, z0);
            glm::vec3 v1(x1, y10, z0);
            glm::vec3 v2(x1, y11, z1);
            glm::vec3 v3(x0, y01, z1);

            
            // Triangle 1: v0, v1, v2
            glm::vec3 edge1_1 = v1 - v0;
            glm::vec3 edge2_1 = v2 - v0;
            glm::vec3 normal1 = -glm::normalize(glm::cross(edge1_1, edge2_1));

            // Triangle 2: v2, v3, v0
            glm::vec3 edge1_2 = v3 - v2;
            glm::vec3 edge2_2 = v0 - v2;
            glm::vec3 normal2 = -glm::normalize(glm::cross(edge1_2, edge2_2));

            // Add first triangle: v0, v1, v2
            terrainData.insert(terrainData.end(), {
                v0.x, v0.y, v0.z, normal1.x, normal1.y, normal1.z,
                v1.x, v1.y, v1.z, normal1.x, normal1.y, normal1.z,
                v2.x, v2.y, v2.z, normal1.x, normal1.y, normal1.z
            });

            // Add second triangle: v2, v3, v0
            terrainData.insert(terrainData.end(), {
                v2.x, v2.y, v2.z, normal2.x, normal2.y, normal2.z,
                v3.x, v3.y, v3.z, normal2.x, normal2.y, normal2.z,
                v0.x, v0.y, v0.z, normal2.x, normal2.y, normal2.z
            });
        }
    }
    return terrainData;
}



std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale)
{
	
    std::vector<float> terrainData;

    terrainData.reserve((width - 1) * (height - 1) * 6 * 6);

    for (int j = 0; j < height - 1; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            float x0 = i * scale;
            float z0 = j * scale;
            float x1 = (i + 1) * scale;
            float z1 = (j + 1) * scale;

            float y00 = 0.0f;
            float y10 = 0.0f;
            float y11 = 0.0f;
            float y01 = 0.0f;

            // Create vertices for the quad
            glm::vec3 v0(x0, y00, z0);
            glm::vec3 v1(x1, y10, z0);
            glm::vec3 v2(x1, y11, z1);
            glm::vec3 v3(x0, y01, z1);

            
            // Triangle 1: v0, v1, v2
            glm::vec3 edge1_1 = v1 - v0;
            glm::vec3 edge2_1 = v2 - v0;
            glm::vec3 normal1 = -glm::normalize(glm::cross(edge1_1, edge2_1));

            // Triangle 2: v2, v3, v0
            glm::vec3 edge1_2 = v3 - v2;
            glm::vec3 edge2_2 = v0 - v2;
            glm::vec3 normal2 = -glm::normalize(glm::cross(edge1_2, edge2_2));

            // Add first triangle: v0, v1, v2
            terrainData.insert(terrainData.end(), {
                v0.x, v0.y, v0.z, normal1.x, normal1.y, normal1.z,
                v1.x, v1.y, v1.z, normal1.x, normal1.y, normal1.z,
                v2.x, v2.y, v2.z, normal1.x, normal1.y, normal1.z
            });

            // Add second triangle: v2, v3, v0
            terrainData.insert(terrainData.end(), {
                v2.x, v2.y, v2.z, normal2.x, normal2.y, normal2.z,
                v3.x, v3.y, v3.z, normal2.x, normal2.y, normal2.z,
                v0.x, v0.y, v0.z, normal2.x, normal2.y, normal2.z
            });
        }
    }
    return terrainData;
}

