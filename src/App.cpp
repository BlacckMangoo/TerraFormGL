#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManager.h"
#include "Light.h"
#include <Mesh.h>
#include "MeshData.h"
#include <iostream>

namespace {
 
    Mesh* sphere = nullptr;

}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
}

App::~App()
{
    // Clean up the objects we created
    
    // Clean up mesh
    if (sphere) {
        delete sphere;
        sphere = nullptr;
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

    //unlitShader
    vertPath = basePath + "shaders/unlitShader.vert";
    fragPath = basePath + "shaders/unlitShader.frag";
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "unlit");

    Shader meshShader = ResourceManager::GetShader("mesh");
    Shader unlitShader = ResourceManager::GetShader("unlit");

    PhysicsProperties spherePhysics;
    Transform sphereTransform;
    sphereTransform.position = glm::vec3(0.0f, 10.0f, -20.0f);
    sphereTransform.scale = glm::vec3(5.0f);
    spherePhysics.mass = 1.0f;
    spherePhysics.velocity = glm::vec3(0.0f, -1.0f, 0.0f);
    spherePhysics.acceleration = glm::vec3(0.0f, -9.81f, 0.0f);

    sphere = new Mesh(sphereTransform, GenerateSphereMeshData(1.0f, 36), nullptr, spherePhysics);


    glEnable(GL_DEPTH_TEST);
}
void App::ProcessInput(float dt)
{

}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);
    sphere->Update(dt);

}

void App::Render()
{
    float time = glfwGetTime(); 

    glEnable(GL_DEPTH_TEST);
    
    // Update camera view matrix
    camera.updateViewMatrix();


    if (sphere) {
        Shader unlitShader = ResourceManager::GetShader("unlit");
        sphere->Draw(camera, unlitShader);
    }

    
}

