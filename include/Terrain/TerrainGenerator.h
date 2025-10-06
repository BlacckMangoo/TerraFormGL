#pragma once
#include<vector>

#include "TerrainStrategy.h"

class TerrainGenerator {

public:
	

	std::vector<float> generateTerrain(int width, int height, float scale, ITerrainHeightStrategy* heightStrategy);

std::vector<float> generateTerrain(int width, int height, float scale) ; 
};
