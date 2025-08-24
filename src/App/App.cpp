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
       glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    // Load shaders
    const std::string basePath = "../resources/shaders/";
    std::vector<std::string> shaderPaths = { "unlitShader","terrain","light","sprite"};

    for ( auto& shaderPath : shaderPaths ) {
        std::string vertPath = basePath + shaderPath + ".vert";
        std::string fragPath = basePath + shaderPath + ".frag";

        ResourceManager::LoadShader(vertPath.c_str(),fragPath.c_str(),nullptr,shaderPath);
    }

    glEnable(GL_DEPTH_TEST);

    terrainRenderer = TerrainRenderer(ResourceManager::GetShader("terrain"));

    lights.clear();
    lights.push_back(new Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))); // White light
    lights.push_back(new Light(glm::vec3(5.0f, 8.0f, 5.0f), glm::vec3(0.0f, 0.8f, 1.0f))); // Blue-cyan light


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
    time += 3.0f* dt;

    GraphFunctionStrategy graphFunc(TerrainHeightFunctions::breathingBlobs(time));
    // regenerate terrain every frame with updated heights
    terrainRenderer.GenerateTerrain(
        terrainRenderer.terrainWindow.terrainWidth,
        terrainRenderer.terrainWindow.terrainHeight,
        terrainRenderer.terrainWindow.terrainScale,
        &graphFunc
    );
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