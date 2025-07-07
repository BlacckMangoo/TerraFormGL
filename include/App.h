#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "imgui.h"

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

    App(unsigned int width, unsigned int height);
    ~App();

	
   
    void ProcessInput(float dt);
    void Init();
    void Update(float dt);
    void Render();
    void RenderUI(); // New method for ImGui
};