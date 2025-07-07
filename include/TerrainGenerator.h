#pragma once
#include<vector>

class TerrainGenerator {

public:
	

	std::vector<float> generateTerrain(int width, int height, float scale, float frequency, 
		float amplitude, int seed, int noiseType, int fractalType, 
		int octaves, float lacunarity, float gain);

};