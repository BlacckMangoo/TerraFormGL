#include "TerrainGenerator.h"
#include "Noise.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<float> TerrainGenerator::generateTerrain(int width, int height, float scale, float frequency,
	float amplitude, float lacunarity, int noiseType, int fractalType, 
    int fractalOctaves, float fractalGain, float fractalWeightedStrength, 
    float fractalPingPongStrength, int cellularDistanceFunction, 
    int cellularReturnType, float cellularJitter, bool useDomainWarp, 
    int domainWarpType, float domainWarpAmp, float domainWarpFreq,
    float time, bool enableDrift)
{
	FastNoiseLite noise;
	
	// Set noise type
    switch(noiseType) {
        case 0: noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); break;
        case 1: noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S); break;
        case 2: noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular); break;
        case 3: noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); break;
        case 4: noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic); break;
        case 5: noise.SetNoiseType(FastNoiseLite::NoiseType_Value); break;
        default: noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); break;
    }
    
    // Set fractal type
    switch(fractalType) {
        case 0: noise.SetFractalType(FastNoiseLite::FractalType_None); break;
        case 1: noise.SetFractalType(FastNoiseLite::FractalType_FBm); break;
        case 2: noise.SetFractalType(FastNoiseLite::FractalType_Ridged); break;
        case 3: noise.SetFractalType(FastNoiseLite::FractalType_PingPong); break;
        default: noise.SetFractalType(FastNoiseLite::FractalType_FBm); break;
    }
    
    // Set basic noise parameters
	noise.SetFrequency(frequency);
	noise.SetFractalLacunarity(lacunarity);
    noise.SetFractalOctaves(fractalOctaves);
    noise.SetFractalGain(fractalGain);
    noise.SetFractalWeightedStrength(fractalWeightedStrength);
    noise.SetFractalPingPongStrength(fractalPingPongStrength);
    
    // Set cellular noise parameters
    if (noiseType == 2) { // Cellular
        switch(cellularDistanceFunction) {
            case 0: noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Euclidean); break;
            case 1: noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq); break;
            case 2: noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan); break;
            case 3: noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid); break;
        }
        
        switch(cellularReturnType) {
            case 0: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue); break;
            case 1: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance); break;
            case 2: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2); break;
            case 3: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Add); break;
            case 4: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Sub); break;
            case 5: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Mul); break;
            case 6: noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Div); break;
        }
        
        noise.SetCellularJitter(cellularJitter);
    }
    
    // Domain warp setup
    FastNoiseLite domainWarp;
    if (useDomainWarp) {
        switch(domainWarpType) {
            case 0: domainWarp.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2); break;
            case 1: domainWarp.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2Reduced); break;
            case 2: domainWarp.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid); break;
        }
        domainWarp.SetDomainWarpAmp(domainWarpAmp);
        domainWarp.SetFrequency(domainWarpFreq);
    }

    std::vector<float> terrainData;

    // Now storing position (3) + normal (3) = 6 floats per vertex
    terrainData.reserve((width - 1) * (height - 1) * 6 * 6);

    for (int j = 0; j < height - 1; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            float x0 = i * scale;
            float z0 = j * scale;
            float x1 = (i + 1) * scale;
            float z1 = (j + 1) * scale;
            
            // Apply domain warp if enabled - warp each corner separately
            float wx0 = x0, wz0 = z0;
            float wx1 = x1, wz1 = z0;
            float wx2 = x1, wz2 = z1;
            float wx3 = x0, wz3 = z1;
            
            if (useDomainWarp) {
                domainWarp.DomainWarp(wx0, wz0);
                domainWarp.DomainWarp(wx1, wz1);
                domainWarp.DomainWarp(wx2, wz2);
                domainWarp.DomainWarp(wx3, wz3);
            }
     
            // Apply time-based drift for fog-like movement
            float timeOffsetX = enableDrift ? time : 0.0f;
            float timeOffsetZ = enableDrift ? time * 0.7f : 0.0f; // Different speed for X and Z for more natural movement
     
            float y00 = noise.GetNoise((useDomainWarp ? wx0 : x0) + timeOffsetX, (useDomainWarp ? wz0 : z0) + timeOffsetZ) * amplitude;
            float y10 = noise.GetNoise((useDomainWarp ? wx1 : x1) + timeOffsetX, (useDomainWarp ? wz1 : z0) + timeOffsetZ) * amplitude;
            float y11 = noise.GetNoise((useDomainWarp ? wx2 : x1) + timeOffsetX, (useDomainWarp ? wz2 : z1) + timeOffsetZ) * amplitude;
            float y01 = noise.GetNoise((useDomainWarp ? wx3 : x0) + timeOffsetX, (useDomainWarp ? wz3 : z1) + timeOffsetZ) * amplitude;

            // Create vertices for the quad
            glm::vec3 v0(x0, y00, z0);
            glm::vec3 v1(x1, y10, z0);
            glm::vec3 v2(x1, y11, z1);
            glm::vec3 v3(x0, y01, z1);

            // Calculate normals for the two triangles
            // Triangle 1: v0, v1, v2
            glm::vec3 edge1_1 = v1 - v0;
            glm::vec3 edge2_1 = v2 - v0;
            glm::vec3 normal1 = glm::normalize(glm::cross(edge1_1, edge2_1));

            // Triangle 2: v2, v3, v0
            glm::vec3 edge1_2 = v3 - v2;
            glm::vec3 edge2_2 = v0 - v2;
            glm::vec3 normal2 = glm::normalize(glm::cross(edge1_2, edge2_2));

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
