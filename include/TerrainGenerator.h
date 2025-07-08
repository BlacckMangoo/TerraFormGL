#pragma once
#include<vector>

class TerrainGenerator {

public:
	

	std::vector<float> generateTerrain(int width, int height, float scale, float frequency, 
		float amplitude, float lacunarity, int noiseType, int fractalType, 
        int fractalOctaves, float fractalGain, float fractalWeightedStrength, 
        float fractalPingPongStrength, int cellularDistanceFunction, 
        int cellularReturnType, float cellularJitter, bool useDomainWarp, 
        int domainWarpType, float domainWarpAmp, float domainWarpFreq, 
        float time, bool enableDrift);

};