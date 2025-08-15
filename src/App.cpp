#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManager.h"
#include "Light.h"
#include <iostream>

namespace {
    TerrainRenderer* terrainRenderer = nullptr;
    std::vector<Light*> lights;
}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
}

App::~App()
{
    // Clean up the objects we created
    if (terrainRenderer) {
        delete terrainRenderer;
        terrainRenderer = nullptr;
    }
    
    // Clean up lights
    for (auto& light : lights) {
        if (light) {
            delete light;
        }
    }
    lights.clear();

    if (terrainRenderer) {
        terrainRenderer->terrainWindow.Close();
    }
    
    if (camera.cameraWindow) {
        camera.cameraWindow->Close();
    }
}

void App::Init()
{
    const std::string basePath = "../resources/";
    std::string vertPath, fragPath;
    
    // Load sprite shader
    vertPath = basePath + "shaders/sprite.vert";
    fragPath = basePath + "shaders/sprite.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "sprite");
    
    // Load terrain shader
    vertPath = basePath + "shaders/terrain.vert";
    fragPath = basePath + "shaders/terrain.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "mesh");
    
    // Load light shader
    vertPath = basePath + "shaders/light.vert";
    fragPath = basePath + "shaders/light.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "light");
    
    Shader meshShader = ResourceManager::GetShader("mesh");
    
    terrainRenderer = new TerrainRenderer(meshShader);
    
    Light* light = new Light(glm::vec3(200.0f, 40.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    lights.push_back(light);
    
    // Generate initial terrain
    if (terrainRenderer) {
        terrainRenderer->GenerateTerrain(50, 50, 1.0f, 0.1f, 5.0f);
    }
    
    glEnable(GL_DEPTH_TEST);
}
void App::ProcessInput(float dt)
{

}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);
   
    if (terrainRenderer->terrainWindow.regenerateTerrain && terrainRenderer) {
        terrainRenderer->GenerateTerrain(
            terrainRenderer->terrainWindow.terrainWidth,
            terrainRenderer->terrainWindow.terrainHeight, 
            terrainRenderer->terrainWindow.terrainScale,
            terrainRenderer->terrainWindow.terrainFrequency, 
            terrainRenderer->terrainWindow.terrainAmplitude
        );
        terrainRenderer->terrainWindow.regenerateTerrain = false;
    }
}

void App::Render()
{
    float time = glfwGetTime(); 

    
    camera.updateViewMatrix();
    
    Shader lightShader = ResourceManager::GetShader("light");
    for (auto& light : lights) {
        if (light) {
            light->Render(lightShader, camera);
        }
    }

    RenderModes mode = RENDER_MODE_WIRE_FRAME;
    switch(terrainRenderer->terrainWindow.renderMode) {
        case 0: mode = RENDER_MODE_WIRE_FRAME; break;
        case 1: mode = RENDER_MODE_FILL; break;
        case 2: mode = RENDER_MODE_POINTS; break;
    }
    
    if (terrainRenderer) {
        terrainRenderer->DrawTerrain(mode, camera, lights);
    }
    
 
    terrainRenderer->terrainWindow.RenderUi(camera);
    if (camera.cameraWindow) {
        camera.cameraWindow->RenderUi(camera);
    }
}

