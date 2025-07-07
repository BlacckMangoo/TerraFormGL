#include "TerrainGenerator.h"
#include "Noise.h"

std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale, float frequency,
	float amplitude, int seed, int noiseType, int fractalType, 
	int octaves, float lacunarity, float gain)
{
	FastNoiseLite noise;
	
	// Set noise type
	switch(noiseType) {
		case 0: noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); break;
		case 1: noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); break;
		case 2: noise.SetNoiseType(FastNoiseLite::NoiseType_Value); break;
		case 3: noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular); break;
		default: noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); break;
	}
	
	// Set fractal type
	switch(fractalType) {
		case 0: noise.SetFractalType(FastNoiseLite::FractalType_None); break;
		case 1: noise.SetFractalType(FastNoiseLite::FractalType_FBm); break;
		case 2: noise.SetFractalType(FastNoiseLite::FractalType_Ridged); break;
		case 3: noise.SetFractalType(FastNoiseLite::FractalType_PingPong); break;
		default: noise.SetFractalType(FastNoiseLite::FractalType_None); break;
	}
	
	noise.SetSeed(seed);
	noise.SetFrequency(frequency);
	noise.SetFractalOctaves(octaves);
	noise.SetFractalLacunarity(lacunarity);
	noise.SetFractalGain(gain);


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

			noiseData[index++] = noise.GetNoise(x0, z0);
           
			float y00 = noise.GetNoise(x0, z0) * amplitude; 
			float y01 = noise.GetNoise(x0, z1) * amplitude; 
			float y10 = noise.GetNoise(x1, z0) * amplitude; 
			float y11 = noise.GetNoise(x1, z1) * amplitude; 

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