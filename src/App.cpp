#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManager.h"
#include "Light.h"
#include <Mesh.h>
#include "MeshData.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <memory>


namespace {
    TerrainRenderer terrainRenderer;
    std::vector<Light*> lights;
}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    camera.cameraWindow = new CameraWindow();
    terrainRenderer.terrainWindow = TerrainWindow();
}

App::~App()
{
    if (camera.cameraWindow) {
        camera.cameraWindow->Close();
        delete camera.cameraWindow;
        camera.cameraWindow = nullptr;
    }
}

void App::Init()
{
       glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    // Load shaders
    const std::string basePath = "../resources/";
    std::string vertPath, fragPath;

    vertPath = basePath + "shaders/sprite.vert";
    fragPath = basePath + "shaders/sprite.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "sprite");

    vertPath = basePath + "shaders/terrain.vert";
    fragPath = basePath + "shaders/terrain.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "mesh");

    vertPath = basePath + "shaders/light.vert";
    fragPath = basePath + "shaders/light.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "light");

    vertPath = basePath + "shaders/unlitShader.vert";
    fragPath = basePath + "shaders/unlitShader.frag";
    Shader unlitShader = ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "unlit");

    vertPath = basePath + "shaders/terrain.vert";
    fragPath = basePath + "shaders/terrain.frag";
    Shader terrainShader = ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "terrain");

    glEnable(GL_DEPTH_TEST);

    terrainRenderer = TerrainRenderer(terrainShader);

    lights.clear();
    lights.push_back(new Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))); // White light
    lights.push_back(new Light(glm::vec3(5.0f, 8.0f, 5.0f), glm::vec3(0.0f, 0.8f, 1.0f))); // Blue-cyan light

    terrainRenderer.GenerateTerrain(
        terrainRenderer.terrainWindow.terrainWidth,
        terrainRenderer.terrainWindow.terrainHeight,
        terrainRenderer.terrainWindow.terrainScale,
        terrainRenderer.terrainWindow.terrainFrequency,
        terrainRenderer.terrainWindow.terrainAmplitude
    );

    for( auto* light : lights) {
        clickables.push_back(light);
    }
}

void App::ProcessInput(float dt)
{
}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);

    if (terrainRenderer.terrainWindow.regenerateTerrain) {
        terrainRenderer.GenerateTerrain(
            terrainRenderer.terrainWindow.terrainWidth,
            terrainRenderer.terrainWindow.terrainHeight,
            terrainRenderer.terrainWindow.terrainScale,
            terrainRenderer.terrainWindow.terrainFrequency,
            terrainRenderer.terrainWindow.terrainAmplitude
        );
        terrainRenderer.terrainWindow.regenerateTerrain = false;
    }
}

void App::Render()
{
    float time = glfwGetTime();
    glEnable(GL_DEPTH_TEST);

    camera.updateViewMatrix();
    camera.cameraWindow->RenderUi(camera);
    terrainRenderer.terrainWindow.RenderUi(camera);
    
    for (auto* l : lights) {
        if (l->lightWindow) {
            l->lightWindow->RenderUi(camera);
        }
    }

    Shader terrainShader = ResourceManager::GetShader("terrain");
    terrainShader.Use();

    std::vector<Light*> lightPtrs;
    for (auto* l : lights) {
        lightPtrs.push_back(l);
    }
    
    terrainRenderer.DrawTerrain(
        terrainRenderer.terrainWindow.GetRenderMode(),
        camera,
        lightPtrs
    );

    Shader lightShader = ResourceManager::GetShader("light");
    lightShader.Use();

    for (auto* l : lights) {
      l->Render(lightShader, camera);
    }
}