#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


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


    App(unsigned int width, unsigned int height);
    ~App();

	
   
    void ProcessInput(float dt);
    void Init();
    void Update(float dt);
    void Render();
};