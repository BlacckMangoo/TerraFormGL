#pragma once

#include <vector>
#include "../Core/Camera.h"
#include "../Core/IClickable.h"

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
    float time = 0.0f;
    
public:
    Camera camera;
    std::vector<IClickable*> clickables;
 


};