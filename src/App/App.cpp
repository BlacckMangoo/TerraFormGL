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
#include "../../include/Terrain/TerrainStrategy.h"
#include "Graphics/MeshData.h"

namespace {

    std::vector<Light*>   lights ;
    TerrainRenderer terrainRenderer ; 
}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    camera.cameraWindow = new CameraWindow();

}

App::~App()
{


    // Cleanup lights
    for (auto* l : lights) {
        delete l;
    }
    lights.clear();

}

void App::Init()
{
 


    // Load shaders first so any subsequent Draw/Use calls have valid shader programs
    const std::string basePath = "../resources/shaders/";
    std::vector<std::string> shaderPaths = { "unlitShader","terrain","light","sprite"};

    for ( auto& shaderPath : shaderPaths ) {
        std::string vertPath = basePath + shaderPath + ".vert";
        std::string fragPath = basePath + shaderPath + ".frag";

        ResourceManager::LoadShader(vertPath.c_str(),fragPath.c_str(),nullptr,shaderPath);
    }

   Shader terrainShader = ResourceManager::GetShader("terrain");
   terrainShader.Use();

   terrainRenderer = TerrainRenderer(terrainShader);

    // Generate initial terrain using breathing blobs (time-based). We'll update it every frame in Update().
    GraphFunctionStrategy graphStrat(TerrainHeightFunctions::breathingBlobs(time, 0.3f, 5));
    // use the App's terrainRenderer member
    terrainRenderer.GenerateTerrain(100, 100, 1.0f, &graphStrat);

    // Create 3 distinct lights around the terrain
    lights.push_back(new Light(glm::vec3(0.0f, 12.0f, 0.0f), glm::vec3(1.0f, 0.9f, 0.7f), 1.2f)); // warm overhead
    lights.push_back(new Light(glm::vec3(12.0f, 6.0f, 6.0f), glm::vec3(0.6f, 0.8f, 1.0f), 0.8f)); // cool side
    lights.push_back(new Light(glm::vec3(-10.0f, 6.0f, -8.0f), glm::vec3(0.6f, 1.0f, 0.6f), 0.8f)); // green rim

    glEnable(GL_DEPTH_TEST);





}

void App::ProcessInput(float dt)
{

}

void App::Update(float dt) {
    camera.processInput(glfwGetCurrentContext(), dt);
    time += 1.0f * dt;

    // Regenerate terrain each frame using the current time so the blobs 'breathe'.
    GraphFunctionStrategy dynamicStrat(TerrainHeightFunctions::breathingBlobs(time, 0.3f, 5));
    terrainRenderer.GenerateTerrain(100, 100, 1.0f, &dynamicStrat);

    Shader terrainShader = ResourceManager::GetShader("terrain");
    terrainShader.Use();

    terrainRenderer.DrawTerrain(RENDER_MODE_WIRE_FRAME, camera, lights);

}

void App::Render()
{

    glEnable(GL_DEPTH_TEST);

    camera.updateViewMatrix();
    camera.cameraWindow->RenderUi(camera);

    Shader lightShader = ResourceManager::GetShader("light");
    lightShader.Use();

    for (auto& light : lights) {
        light->Render(lightShader,camera);
    }




}