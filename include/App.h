#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Camera.h"
#include "TerrainWindow.h"
#include "CameraWindow.h"
#include "IClickable.h"
// Forward declaration
class Mesh;

class App
{
public:
    unsigned int Width, Height;
    App(unsigned int width, unsigned int height);
    ~App();
    
    // Core app loop methods
    void ProcessInput(float dt);
    void Init();
    void Update(float dt);
    void Render();
    
public:
    Camera camera;
    std::vector<IClickable*> clickables;

};