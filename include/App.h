#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "imgui.h"
#include "Light.h"


class App
{
public:

    unsigned int            Width, Height;
    App(unsigned int width, unsigned int height) ;

    ~App();
    void ProcessInput(float dt);
    void Init();
    void Update(float dt);
    void Render();
    
};