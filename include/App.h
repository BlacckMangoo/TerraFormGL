#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "imgui.h"
#include "Light.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};





class App
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;

    // Terrain parameters for ImGui controls
    int terrainWidth = 50;
    int terrainHeight = 50;
    float terrainScale = 1.0f;
    float terrainFrequency = 0.1f;
    float terrainAmplitude = 4.0f;
    float fractalLacunarity = 2.0f;
    bool regenerateTerrain = false;
    int renderMode = 0; // 0=wireframe, 1=fill, 2=points

    // Advanced Noise Parameters
    int noiseType = 0; // 0=OpenSimplex2, 1=OpenSimplex2S, 2=Cellular, 3=Perlin, 4=ValueCubic, 5=Value
    int fractalType = 1; // 0=None, 1=FBm, 2=Ridged, 3=PingPong
    int fractalOctaves = 3;
    float fractalGain = 0.5f;
    float fractalWeightedStrength = 0.0f;
    float fractalPingPongStrength = 2.0f;
    
    // Cellular noise specific
    int cellularDistanceFunction = 0; // 0=Euclidean, 1=EuclideanSq, 2=Manhattan, 3=Hybrid
    int cellularReturnType = 1; // 0=CellValue, 1=Distance, 2=Distance2, 3=Distance2Add, etc.
    float cellularJitter = 1.0f;
    
    // Domain warp
    bool useDomainWarp = false;
    int domainWarpType = 0;
    float domainWarpAmp = 30.0f;
    float domainWarpFreq = 0.05f;

    // Time-based noise drift
    float noiseTime = 0.0f;
    float noiseDriftSpeed = 0.02f; // Control speed of drift
    bool enableNoiseDrift = false;

    // Light parameters for ImGui controls
    float lightPosX = 0.0f;
    float lightPosY = 50.0f;
    float lightPosZ = 0.0f;
    float lightColorR = 1.0f;
    float lightColorG = 1.0f;
    float lightColorB = 1.0f;
    float lightScale = 2.0f;

    // Light visualization
    Light* lightSource;

    App(unsigned int width, unsigned int height);
    ~App();

	
   
    void ProcessInput(float dt);
    void Init();
    void Update(float dt);
    void Render();
    void RenderUI(); // New method for ImGui
};