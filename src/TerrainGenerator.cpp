#include "TerrainGenerator.h"
#include "Noise.h"

std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale, float frequency,
	float amplitude, float lacunarity)
{
	FastNoiseLite noise;
	
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFrequency(frequency);
	noise.SetFractalLacunarity(lacunarity);

    std::vector<float> terrainData;
	std::vector<float> noiseData((width-1)*(height-1));
    int index = 0; 

    terrainData.reserve((width - 1) * (height - 1) * 6 * 3);

    for (int j = 0; j < height - 1; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            float x0 = i * scale;
            float z0 = j * scale;
            float x1 = (i + 1) * scale;
            float z1 = (j + 1) * scale;

            float y00 = noise.GetNoise(x0, z0) * amplitude;
            float y01 = noise.GetNoise(x0, z1) * amplitude;
            float y10 = noise.GetNoise(x1, z0) * amplitude;
            float y11 = noise.GetNoise(x1, z1) * amplitude;

            terrainData.insert(terrainData.end(), {
                x0, y00, z0,
                x1, y10, z0,
                x1, y11, z1,

                x1, y11, z1,
                x0, y01, z1,
                x0, y00, z0
                });
        }
    }
    return terrainData;
}