#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManger.h"
#include <UiManager.h>
#include "Light.h"

#include <glm/glm.hpp>

TerrainRenderer* terrainRenderer;
Camera camera; 
std::vector<Light*> lights;
std::vector<glm::vec3> lightColor = {glm::vec3(1.0f, 0.5f, 0.5f), // Light 1 color
	glm::vec3(0.5f, 1.0f, 0.5f), // Light 2 color
	glm::vec3(0.5f, 0.5f, 1.0f),  // Light 3 color
	glm::vec3(1.0f, 1.0f, 0.5f) // Light 4 color
};
std::vector<glm::vec3>lightPosition = {
	glm::vec3(7.0f, 10.0f, 40.0f),
	glm::vec3(60.0f, 10.0f, 100.0f),
	glm::vec3(80.0f, 10.0f, 80.0f),
	glm::vec3(50.0f, 10.0f, 60.0f)
};;

UiManager uiManager;



App::App(unsigned int width, unsigned int height)
    : Width(width), Height(height)
{

}

App::~App()
{
	//init ui 
	uiManager.Close();
}

void App::Init()
{

	ResourceManager::LoadShader(RESOURCES_PATH "shaders/sprite.vert", RESOURCES_PATH "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/terrain.vert", RESOURCES_PATH "shaders/terrain.frag", nullptr, "mesh");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/light.vert", RESOURCES_PATH "shaders/light.frag", nullptr, "light");
	
	Shader meshShader = ResourceManager::GetShader("mesh");

	

	void* raw = operator new(sizeof(TerrainRenderer));
	TerrainRenderer* terrainRenderer = static_cast<TerrainRenderer*>(raw);
	new (terrainRenderer) TerrainRenderer(meshShader);


	for (int i = 0; i < lightColor.size(); i++) {
		Light* light = new Light(lightPosition[i], lightColor[i], 1.0f);
		lights.push_back(light);
	};
	
	
	

	
	// Generate initial terrain using class parameters
	terrainRenderer->GenerateTerrain(uiManager.terrainWidth, uiManager.terrainHeight, uiManager.terrainAmplitude,
		uiManager.terrainScale, uiManager.terrainFrequency);
	
	glEnable(GL_DEPTH_TEST);

}
void App::Update(float dt)
{
	camera.processInput(glfwGetCurrentContext(),dt);



	// Check if terrain needs to be regenerated
	if (uiManager.regenerateTerrain) {
		// Regenerate terrain with current parameters
		terrainRenderer->GenerateTerrain(uiManager.terrainWidth, uiManager.terrainHeight, uiManager.terrainScale,
			uiManager.terrainFrequency, uiManager.terrainAmplitude);
		uiManager.regenerateTerrain = false; // Reset flag
	}
}

void App::ProcessInput(float dt)
{

}

void App::Render()
{
	float time = glfwGetTime(); // Total time in seconds

	camera.updateViewMatrix();
	
	// Convert render mode to RenderModes enum
	RenderModes mode = RENDER_MODE_WIRE_FRAME;
	switch(uiManager.renderMode) {
		case 0: mode = RENDER_MODE_WIRE_FRAME; break;
		case 1: mode = RENDER_MODE_FILL; break;
		case 2: mode = RENDER_MODE_POINTS; break;
	}
	terrainRenderer->DrawTerrain(mode, camera, lights);
	
	// Render the light source cube
	Shader lightShader = ResourceManager::GetShader("light");
	for (Light* light : lights) {
		light->Render(lightShader, camera);
	}
	
	
	uiManager.RenderUi(camera);
}

