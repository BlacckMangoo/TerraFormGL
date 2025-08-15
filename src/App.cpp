#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManager.h"
#include "Light.h"
#include <Mesh.h>
#include "MeshData.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace {

    std::vector<Mesh*> spheres = {nullptr};}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
}

App::~App()
{
   
    for (auto& s : spheres) {
        delete s;
    }
    spheres.clear();

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


   // Initialize random seed
   srand(static_cast<unsigned int>(time(nullptr)));

   for (int i = 0; i < 2000; i++)
   {
       Transform sphereTransform;

       sphereTransform.position = glm::vec3(
           (rand() % 2000 - 1000) / 100.0f,  // -10 to +10
           (rand() % 2000 - 1000) / 100.0f,  // -10 to +10
           (rand() % 2000 - 1000) / 100.0f   // -10 to +10
       );
       sphereTransform.scale = glm::vec3(0.008f);
       

       PhysicsProperties physics;
       physics.velocity = glm::vec3(
           (rand() % 1000 - 500) / 100.0f,   // -5 to +5
           (rand() % 1000 - 500) / 100.0f,   // -5 to +5
           (rand() % 1000 - 500) / 100.0f    // -5 to +5
       );
       physics.acceleration = glm::vec3(0.0f, -0.9f, 0.0f); // Light gravity
       
       spheres.push_back(new Mesh(sphereTransform, GenerateSphereMeshData(1.0f, 32), nullptr, physics));
   }

    glEnable(GL_DEPTH_TEST);
}
void App::ProcessInput(float dt)
{

}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);
    
    // Define boundary box
    const float boundarySize = 35.0f;
    
    for (auto& s : spheres) {
        if (s) {
            s->Update(dt);
            
         
            for (int i = 0; i < 3; i++) {
            
                if (std::abs(s->transform.position[i]) > boundarySize) {
             
                    s->physicsProperties.velocity[i] = -s->physicsProperties.velocity[i] * 0.8f;
                    
                  
                    s->transform.position[i] = boundarySize * (s->transform.position[i] > 0 ? 0.99f : -0.99f);
                }
            }
        }
    }
}

void App::Render()
{
    float time = glfwGetTime(); 

    glEnable(GL_DEPTH_TEST);

    camera.updateViewMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (auto& s : spheres) {
        if (s) {
            Shader unlitShader = ResourceManager::GetShader("unlit");
            s->Draw(camera, unlitShader);
        }
    }


}

