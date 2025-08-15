#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManager.h"
#include <UiManager.h>
#include "Light.h"
#include <iostream>










// Light positions and colors
std::vector<glm::vec3> lightPosition = {
	glm::vec3(20.0f, 4.0f, 2.0f),
	glm::vec3(280.0f, 3.0f, -1.0f),
	glm::vec3(60.0f, 5.0f, 0.0f),

};

std::vector<glm::vec3> lightColor = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),

};

App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height), terrainRenderer(nullptr)
{

}

App::~App()
{
	// Clean up terrain renderer
	if (terrainRenderer) {
		delete terrainRenderer;
	}
	
	// Clean up lights
	for (Light* light : lights) {
		delete light;
	}
	
	//init ui 
	uiManager.Close();
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

	// Create terrain renderer
	terrainRenderer = new TerrainRenderer(meshShader);

	// Create lights
	for (int i = 0; i < lightColor.size(); i++) {
		Light* light = new Light(lightPosition[i], lightColor[i]);
		lights.push_back(light);
	};
	
	
	glEnable(GL_DEPTH_TEST);

}
void App::Update(float dt)
{
	camera.processInput(glfwGetCurrentContext(),dt);

	
	if (uiManager.regenerateTerrain) {
	
		terrainRenderer->GenerateTerrain(uiManager.terrainWidth, uiManager.terrainHeight, uiManager.terrainScale,
			uiManager.terrainFrequency, uiManager.terrainAmplitude);
		uiManager.regenerateTerrain = false; 
	}
}

void App::ProcessInput(float dt)
{

}

void App::Render()
{
	float time = glfwGetTime(); 

	camera.updateViewMatrix();
	
	RenderModes mode = RENDER_MODE_WIRE_FRAME;
	switch(uiManager.renderMode) {
		case 0: mode = RENDER_MODE_WIRE_FRAME; break;
		case 1: mode = RENDER_MODE_FILL; break;
		case 2: mode = RENDER_MODE_POINTS; break;
	}
	terrainRenderer->DrawTerrain(mode, camera, lights);
	
	
	Shader lightShader = ResourceManager::GetShader("light");
	for (Light* light : lights) {
		light->Render(lightShader, camera);
	}
	
	uiManager.RenderUi(camera);
}

