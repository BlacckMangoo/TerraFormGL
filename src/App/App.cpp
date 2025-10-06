#include "../../include/App/App.h"
#include "../../include/Terrain/TerrainRenderer.h"
#include "../../include/Utils/ResourceManager.h"
#include "../../include/Graphics/Light.h"
#include <../../include/Graphics/Mesh.h>
#include <../../include/Core/CameraWindow.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <memory>
#include "../../include/Terrain/TerrainHeightFunctions.h"

namespace {
    TerrainRenderer terrainRenderer;
    std::vector<Light*> lights;

}

void App::LoadShaders()
{
    const std::string basePath = "../resources/shaders/";
    std::vector<std::string> shaderPaths = { "unlitShader","terrain","light","sprite","compute"};

    for ( auto& shaderPath : shaderPaths ) {
        std::string vertPath = basePath + shaderPath + ".vert";
        std::string fragPath = basePath + shaderPath + ".frag";
        std::string compPath = basePath + shaderPath + ".glsl";

       ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, shaderPath);
    }

    ResourceManager::LoadComputeShader("../resources/shaders/compute.glsl","compute");
}
App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    camera.cameraWindow = new CameraWindow();
    terrainRenderer.terrainWindow = TerrainWindow();
}

App::~App()
{

}

void App::Init()
{

   App::LoadShaders();
    glEnable(GL_DEPTH_TEST);

    terrainRenderer = TerrainRenderer(ResourceManager::GetShader("terrain"),ResourceManager::GetShader("compute"));
    lights.push_back(new Light(glm::vec3(20.0f, 4.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f))); // White light

    for( auto* light : lights) {
        clickables.push_back(light);
    }

    terrainRenderer.GenerateTerrain(
   terrainRenderer.terrainWindow.terrainWidth,
   terrainRenderer.terrainWindow.terrainHeight,
   terrainRenderer.terrainWindow.terrainScale
);

}

void App::ProcessInput(float dt)
{
}

void App::Update(float dt) {
    camera.processInput(glfwGetCurrentContext(), dt);
    time += 1.0f * dt;

    // Check if terrain needs to be regenerated
    if (terrainRenderer.terrainWindow.regenerateTerrain) {
        terrainRenderer.GenerateTerrain(
            terrainRenderer.terrainWindow.terrainWidth,
            terrainRenderer.terrainWindow.terrainHeight,
            terrainRenderer.terrainWindow.terrainScale
        );
        // Reset the flag
        terrainRenderer.terrainWindow.regenerateTerrain = false;
    }
}

void App::Render()
{



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