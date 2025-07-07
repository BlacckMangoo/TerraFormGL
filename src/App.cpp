#include "App.h"
#include "MeshRenderer.h"
#include "ResourceManger.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

MeshRenderer* meshRenderer;
Camera camera; 





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

	// Cleanup mesh renderer
	delete meshRenderer;
}

void App::Init()
{

	ResourceManager::LoadShader(RESOURCES_PATH "shaders/sprite.vert", RESOURCES_PATH "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/mesh.vert", RESOURCES_PATH "shaders/mesh.frag", nullptr, "mesh");
	
	Shader meshShader = ResourceManager::GetShader("mesh");

	
	meshRenderer = new MeshRenderer(meshShader);
	
	// Generate initial terrain using class parameters
	meshRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
		terrainAmplitude, fractalLacunarity);
	
	glEnable(GL_DEPTH_TEST);

}


void App::Update(float dt)
{
	camera.processInput(glfwGetCurrentContext(),dt);

	// Check if terrain needs to be regenerated
	if (regenerateTerrain) {
		meshRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
			terrainAmplitude, fractalLacunarity);
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
	
	meshRenderer->DrawTerrain(mode, time, camera);
	
	// Render ImGui UI
	RenderUI();
}

void App::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
	
	ImGui::Begin("Controls");
	
	ImGui::Text("Terrain Parameters");
	ImGui::Separator();
	
	if (ImGui::SliderInt("Width", &terrainWidth, 1, 1000)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderInt("Height", &terrainHeight, 1, 1000)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Scale", &terrainScale, 0.1f, 1.0f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Frequency", &terrainFrequency, 0.01f, 0.2f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Amplitude", &terrainAmplitude, 0.01f, 2.0f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Lacunarity", &fractalLacunarity, 1.0f, 4.0f)) {
		regenerateTerrain = true;
	}
	
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
	
	ImGui::End();
}