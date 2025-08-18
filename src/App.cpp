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

const constexpr float GravitationalConstant = 3.0f;

namespace {

}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    camera.cameraWindow = new CameraWindow();
}

App::~App()
{
  
    
    if (camera.cameraWindow) {
        camera.cameraWindow->Close();
    }
}

void App::Init()
{
    //load shaders 
    {
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
    }

    glEnable(GL_DEPTH_TEST);


}

void App::ProcessInput(float dt)
{
}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);
}

void App::Render()
{
    float time = glfwGetTime(); 
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    camera.updateViewMatrix();
    camera.cameraWindow->RenderUi(camera);
    
    Shader unlitShader = ResourceManager::GetShader("unlit");


}