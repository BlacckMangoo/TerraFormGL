#include "App.h"
#include "TerrainRenderer.h"
#include "ResourceManger.h"
#include "Camera.h"
#include "Light.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>

TerrainRenderer* terrainRenderer;
Camera camera; 
std::vector<Light*> lights;
glm::vec3 lighcolor[3] = { glm::vec3(1.0f, 0.5f, 0.5f), // Light 1 color
	glm::vec3(0.5f, 1.0f, 0.5f), // Light 2 color
	glm::vec3(0.5f, 0.5f, 1.0f)  // Light 3 color
};
glm::vec3 lightPosition[3] = {
	glm::vec3(0.0f, 10.0f, 0.0f),
	glm::vec3(10.0f, 10.0f, 10.0f),
	glm::vec3(-10.0f, 10.0f, -10.0f)
};;



App::App(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

App::~App()
{
	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Cleanup renderers
	delete terrainRenderer;
	

}

void App::Init()
{

	ResourceManager::LoadShader(RESOURCES_PATH "shaders/sprite.vert", RESOURCES_PATH "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/terrain.vert", RESOURCES_PATH "shaders/terrain.frag", nullptr, "mesh");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/light.vert", RESOURCES_PATH "shaders/light.frag", nullptr, "light");
	
	Shader meshShader = ResourceManager::GetShader("mesh");

	
	terrainRenderer = new TerrainRenderer(meshShader);

	for (int i = 0; i < 3; i++) {
		Light* light = new Light(lightPosition[i], lighcolor[i], 1.0f);
		lights.push_back(light);
	};
	
	
	

	
	// Generate initial terrain using class parameters
	terrainRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
		terrainAmplitude);
	
	glEnable(GL_DEPTH_TEST);

}
void App::Update(float dt)
{
	camera.processInput(glfwGetCurrentContext(),dt);



	// Check if terrain needs to be regenerated
	if (regenerateTerrain) {
		terrainRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
			terrainAmplitude);
		regenerateTerrain = false;
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
	switch(renderMode) {
		case 0: mode = RENDER_MODE_WIRE_FRAME; break;
		case 1: mode = RENDER_MODE_FILL; break;
		case 2: mode = RENDER_MODE_POINTS; break;
	}
	

	
	terrainRenderer->DrawTerrain(mode, time, camera, lights);
	
	// Render the light source cube
	Shader lightShader = ResourceManager::GetShader("light");
	for (Light* light : lights) {
		light->Render(lightShader, camera);
	}
	
	
	// Render ImGui UI
	RenderUI();
}

void App::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(380, 700), ImGuiCond_FirstUseEver);
	
	ImGui::Begin("Terrain Controls");
	
	ImGui::Text("Basic Terrain Parameters");
	ImGui::Separator();
	
	if (ImGui::SliderInt("Width", &terrainWidth, 1, 200)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderInt("Height", &terrainHeight, 1, 200)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Scale", &terrainScale, 0.1f, 5.0f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Frequency", &terrainFrequency, 0.01f, 0.5f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Amplitude", &terrainAmplitude, 0.0f, 20.0f)) {
		regenerateTerrain = true;
	}

	
	
	
	ImGui::Separator();
	ImGui::Text("Render Settings");
	ImGui::Separator();
	
	const char* renderModes[] = { "Wireframe", "Fill", "Points" };
	ImGui::Combo("Render Mode", &renderMode, renderModes, 3);
	
	if (ImGui::Button("Regenerate Terrain", ImVec2(-1, 0))) {
		regenerateTerrain = true;
	}

	
	
	
	if (ImGui::Button("Reset Camera", ImVec2(-1, 0))) {
		camera.cameraPos = glm::vec3(25.0f, 15.0f, 25.0f);
		camera.cameraFront = glm::vec3(-0.5f, -0.3f, -0.5f);
		camera.yaw = -90.0f;
		camera.pitch = 0.0f;
		camera.fov = 45.0f;
		camera.firstMouse = true;
	}

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	
	ImGui::End();
}