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

const constexpr float GravitationalConstant = 3.0f;

namespace {
    std::vector<Mesh*> spheres;
    Mesh* spaceTimeGrid = nullptr;
    Mesh* blackHole = nullptr;
    std::default_random_engine generator;
}

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{
    generator.seed(time(nullptr));
}

App::~App()
{
    for (auto& s : spheres) {
        delete s;
    }
    spheres.clear();
    
    if (spaceTimeGrid) {
        delete spaceTimeGrid;
        spaceTimeGrid = nullptr;
    }
    
    if (blackHole) {
        delete blackHole;
        blackHole = nullptr;
    }
    
    if (camera.cameraWindow) {
        camera.cameraWindow->Close();
    }
}

void App::Init()
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
    ResourceManager::LoadShader(vertPath.c_str(), fragPath.c_str(), nullptr, "unlit");
    
    // Create central black hole
    PhysicsProperties blackHolePhysics;
    blackHolePhysics.mass = 75.0f;
    blackHolePhysics.velocity = glm::vec3(0.0f);
    Transform blackHoleTransform;
    blackHoleTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    blackHoleTransform.scale = glm::vec3(0.4f);
    
    blackHole = new Mesh(blackHoleTransform, GenerateSphereMeshData(1.0f, 20), nullptr, blackHolePhysics, glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
    
    // Create massive accretion disk with thousands of particles
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
    std::uniform_real_distribution<float> colorVariation(0.6f, 1.0f);
    std::uniform_real_distribution<float> radiusVariation(-0.3f, 0.3f);
    std::uniform_real_distribution<float> heightVariation(-0.2f, 0.2f);
    
    // Ultra-dense inner hot ring (blazing blue-white) - INNERMOST STABLE CIRCULAR ORBIT
    for (int i = 0; i < 2400; i++) {
        float baseAngle = (float)i / 2400.0f * 2.0f * 3.14159f;
        float angle = baseAngle + angleDist(generator) * 0.1f;
        float radius = 1.8f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.98f;
        
        PhysicsProperties physics;
        physics.mass = 0.02f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator), sin(angle) * radius);
        transform.scale = glm::vec3(0.015f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(0.9f * intensity, 0.95f * intensity, 1.0f * intensity, 1.0f)));
    }
    
    // Dense inner ring (white-hot)
    for (int i = 0; i < 3600; i++) {
        float baseAngle = (float)i / 3600.0f * 2.0f * 3.14159f;
        float angle = baseAngle + angleDist(generator) * 0.15f;
        float radius = 2.8f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.96f;
        
        PhysicsProperties physics;
        physics.mass = 0.025f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator), sin(angle) * radius);
        transform.scale = glm::vec3(0.018f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(1.0f * intensity, 0.9f * intensity, 0.8f * intensity, 1.0f)));
    }
    
    // Main accretion disk - hot yellow ring
    for (int i = 0; i < 6000; i++) {
        float baseAngle = (float)i / 6000.0f * 2.0f * 3.14159f;
        float angle = baseAngle + angleDist(generator) * 0.2f;
        float radius = 4.0f + (i % 600) * 0.01f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.93f;
        
        PhysicsProperties physics;
        physics.mass = 0.03f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator), sin(angle) * radius);
        transform.scale = glm::vec3(0.02f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(1.0f * intensity, 0.8f * intensity, 0.4f * intensity, 1.0f)));
    }
    
    // Extended warm orange ring
    for (int i = 0; i < 5400; i++) {
        float baseAngle = (float)i / 5400.0f * 2.0f * 3.14159f;
        float angle = baseAngle + angleDist(generator) * 0.25f;
        float radius = 6.0f + (i % 750) * 0.008f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.90f;
        
        PhysicsProperties physics;
        physics.mass = 0.035f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator), sin(angle) * radius);
        transform.scale = glm::vec3(0.022f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(1.0f * intensity, 0.6f * intensity, 0.25f * intensity, 1.0f)));
    }
    
    // Outer cooler red ring - thicker disk
    for (int i = 0; i < 4500; i++) {
        float baseAngle = (float)i / 4500.0f * 2.0f * 3.14159f;
        float angle = baseAngle + angleDist(generator) * 0.3f;
        float radius = 8.0f + (i % 900) * 0.006f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.87f;
        
        PhysicsProperties physics;
        physics.mass = 0.04f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator) * 1.5f, sin(angle) * radius);
        transform.scale = glm::vec3(0.025f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(0.9f * intensity, 0.3f * intensity, 0.1f * intensity, 1.0f)));
    }
    
    // Sparse outer dark red edge
    for (int i = 0; i < 2400; i++) {
        float angle = angleDist(generator);
        float radius = 11.0f + (i % 450) * 0.005f + radiusVariation(generator);
        float orbitalSpeed = sqrt(GravitationalConstant * blackHolePhysics.mass / radius) * 0.83f;
        
        PhysicsProperties physics;
        physics.mass = 0.05f;
        physics.velocity = glm::vec3(-sin(angle) * orbitalSpeed, 0.0f, cos(angle) * orbitalSpeed);
        Transform transform;
        transform.position = glm::vec3(cos(angle) * radius, heightVariation(generator) * 2.0f, sin(angle) * radius);
        transform.scale = glm::vec3(0.028f);
        
        float intensity = colorVariation(generator);
        spheres.push_back(new Mesh(transform, GenerateSphereMeshData(1.0f, 8), nullptr, physics, 
                                 glm::vec4(0.7f * intensity, 0.2f * intensity, 0.05f * intensity, 1.0f)));
    }
    
 
    glEnable(GL_DEPTH_TEST);

}

void App::ProcessInput(float dt)
{
}

void App::Update(float dt)
{
    camera.processInput(glfwGetCurrentContext(), dt);
    
    // Clamp timestep for stability with MASSIVE particle count
    const float maxDt = 0.003f;
    if (dt > maxDt) dt = maxDt;
    
    // Optimized gravitational force calculation
    const float eventHorizonRadius = 0.4f;
    const float minParticleDistance = 0.1f;
    
    // Black hole gravitational influence
    if (blackHole) {
        for (auto& particle : spheres) {
            particle->physicsProperties.acceleration = glm::vec3(0.0f);
            
            glm::vec3 direction = blackHole->transform.position - particle->transform.position;
            float distance = glm::length(direction);
            
            if (distance > eventHorizonRadius) {
                // Apply gravitational force from black hole
                glm::vec3 force = direction * (GravitationalConstant * blackHole->physicsProperties.mass / 
                                             (distance * distance * distance));
                particle->physicsProperties.acceleration += force;
                
                // Add slight orbital decay for realism (Hawking radiation effect)
                glm::vec3 tangentialVelocity = particle->physicsProperties.velocity - 
                    glm::dot(particle->physicsProperties.velocity, glm::normalize(direction)) * glm::normalize(direction);
                particle->physicsProperties.acceleration -= tangentialVelocity * 0.01f;
                
            } else {
                // Particle crossed event horizon - respawn it randomly
                std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
                std::uniform_real_distribution<float> radiusDist(12.0f, 18.0f);
                std::uniform_real_distribution<float> heightDist(-0.5f, 0.5f);
                
                float newAngle = angleDist(generator);
                float newRadius = radiusDist(generator);
                float newOrbitalSpeed = sqrt(GravitationalConstant * blackHole->physicsProperties.mass / newRadius) * 0.8f;
                
                particle->transform.position = glm::vec3(cos(newAngle) * newRadius, heightDist(generator), sin(newAngle) * newRadius);
                particle->physicsProperties.velocity = glm::vec3(-sin(newAngle) * newOrbitalSpeed, 0.0f, cos(newAngle) * newOrbitalSpeed);
                
                // Add some random initial velocity for chaos
                std::uniform_real_distribution<float> velocityNoise(-0.2f, 0.2f);
                particle->physicsProperties.velocity += glm::vec3(velocityNoise(generator), velocityNoise(generator), velocityNoise(generator));
            }
        }
    }
    
 
    for (size_t i = 0; i < spheres.size(); i++) {
        for (size_t j = i + 1; j < spheres.size() && j < i + 5; j++) { 
            glm::vec3 direction = spheres[j]->transform.position - spheres[i]->transform.position;
            float distance = glm::length(direction);
            
            if (distance > minParticleDistance && distance < 2.0f) { // Only close interactions
                float forceMagnitude = GravitationalConstant * 0.05f * spheres[i]->physicsProperties.mass * spheres[j]->physicsProperties.mass / 
                                     (distance * distance * distance);
                glm::vec3 force = direction * forceMagnitude;
                
                spheres[i]->physicsProperties.acceleration += force / spheres[i]->physicsProperties.mass;
                spheres[j]->physicsProperties.acceleration -= force / spheres[j]->physicsProperties.mass;
            }
        }
    }
    
    // Update particle physics
    for (auto& particle : spheres) {
        particle->physicsProperties.velocity += particle->physicsProperties.acceleration * dt;
        particle->transform.position += particle->physicsProperties.velocity * dt;
    }
    
    // Update black hole (it can slowly rotate for visual effect)
    if (blackHole) {
        blackHole->transform.rotation.y += 0.5f * dt;
        blackHole->Update(dt);
    }
    
 
}

void App::Render()
{
    float time = glfwGetTime(); 
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    camera.updateViewMatrix();
    camera.cameraWindow->RenderUi(camera);
    
    Shader unlitShader = ResourceManager::GetShader("unlit");
    
    // Render black hole
    if (blackHole) {
        blackHole->Draw(camera, unlitShader);
    }
    
    // Render orbiting particles
    for (auto& particle : spheres) {
        if (particle) {
            particle->Draw(camera, unlitShader);
        }
    }
    
   
}