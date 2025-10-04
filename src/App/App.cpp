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
#include "Graphics/MeshData.h"

namespace {
    TerrainRenderer terrainRenderer;
    std::vector<Light*> lights;
}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    camera.cameraWindow = new CameraWindow();

}

App::~App()
{
    // Cleanup allocated mesh
    if (circleMesh) {
        delete circleMesh;
        circleMesh = nullptr;
    }

    // Cleanup lights
    for (auto* l : lights) {
        delete l;
    }
    lights.clear();

}

void App::Init()
{

    // Load shaders
    const std::string basePath = "../resources/shaders/";
    std::vector<std::string> shaderPaths = { "unlitShader","terrain","light","sprite"};

    for ( auto& shaderPath : shaderPaths ) {
        std::string vertPath = basePath + shaderPath + ".vert";
        std::string fragPath = basePath + shaderPath + ".frag";

        ResourceManager::LoadShader(vertPath.c_str(),fragPath.c_str(),nullptr,shaderPath);
    }

    glEnable(GL_DEPTH_TEST);


    lights.push_back(new Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))); // White light
    // Create debug circle mesh now that GL context should be initialized
    MeshData circle = GenerateCircleMeshData(10.0f, 100, glm::vec3(0,0,0));
    circleMesh = new Mesh(Transform{glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0.1,0.1,0.1)}, circle);


}

void App::ProcessInput(float dt)
{

}

void App::Update(float dt) {
    camera.processInput(glfwGetCurrentContext(), dt);
    time += 1.0f * dt;

    circleMesh->Update(dt);


}

void App::Render()
{

    glEnable(GL_DEPTH_TEST);

    camera.updateViewMatrix();
    camera.cameraWindow->RenderUi(camera);


    Shader lightShader = ResourceManager::GetShader("unlitShader");
    lightShader.Use();
    circleMesh->Draw(camera,lightShader);


}