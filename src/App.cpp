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
	delete lightSource;

}

void App::Init()
{

	ResourceManager::LoadShader(RESOURCES_PATH "shaders/sprite.vert", RESOURCES_PATH "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/terrain.vert", RESOURCES_PATH "shaders/terrain.frag", nullptr, "mesh");
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/light.vert", RESOURCES_PATH "shaders/light.frag", nullptr, "light");
	
	Shader meshShader = ResourceManager::GetShader("mesh");

	
	terrainRenderer = new TerrainRenderer(meshShader);
	
	// Initialize light source
	lightSource = new Light(glm::vec3(lightPosX, lightPosY, lightPosZ), 
	                       glm::vec3(lightColorR, lightColorG, lightColorB), 
	                       2.0f);
	lightSource->Init();
	
	// Generate initial terrain using class parameters
	terrainRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
		terrainAmplitude, fractalLacunarity, noiseType, fractalType, fractalOctaves, 
        fractalGain, fractalWeightedStrength, fractalPingPongStrength, 
        cellularDistanceFunction, cellularReturnType, cellularJitter, 
        useDomainWarp, domainWarpType, domainWarpAmp, domainWarpFreq,
        noiseTime, enableNoiseDrift);
	
	glEnable(GL_DEPTH_TEST);

}
void App::Update(float dt)
{
	camera.processInput(glfwGetCurrentContext(),dt);

	// Update noise time for drift effect
	if (enableNoiseDrift) {
		noiseTime += dt * noiseDriftSpeed;
		regenerateTerrain = true; // Continuously regenerate when drifting
	}

	// Update light position and color from ImGui controls
	lightSource->SetPosition(glm::vec3(lightPosX, lightPosY, lightPosZ));
	lightSource->SetColor(glm::vec3(lightColorR, lightColorG, lightColorB));
	lightSource->SetScale(lightScale);

	// Check if terrain needs to be regenerated
	if (regenerateTerrain) {
		terrainRenderer->GenerateTerrain(terrainWidth, terrainHeight, terrainScale, terrainFrequency,
			terrainAmplitude, fractalLacunarity, noiseType, fractalType, fractalOctaves, 
            fractalGain, fractalWeightedStrength, fractalPingPongStrength, 
            cellularDistanceFunction, cellularReturnType, cellularJitter, 
            useDomainWarp, domainWarpType, domainWarpAmp, domainWarpFreq,
            noiseTime, enableNoiseDrift);
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
	
	// Create light vectors from the ImGui controlled variables
	glm::vec3 lightPos(lightPosX, lightPosY, lightPosZ);
	glm::vec3 lightColor(lightColorR, lightColorG, lightColorB);
	
	terrainRenderer->DrawTerrain(mode, time, camera, lightPos, lightColor);
	
	// Render the light source cube
	Shader lightShader = ResourceManager::GetShader("light");
	lightSource->Render(lightShader, camera);
	
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
	if (ImGui::SliderFloat("Lacunarity", &fractalLacunarity, 1.0f, 4.0f)) {
		regenerateTerrain = true;
	}
	
	ImGui::Separator();
	ImGui::Text("Noise Type & Fractal Settings");
	ImGui::Separator();
	
	const char* noiseTypes[] = { "OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "ValueCubic", "Value" };
	if (ImGui::Combo("Noise Type", &noiseType, noiseTypes, 6)) {
		regenerateTerrain = true;
	}
	
	const char* fractalTypes[] = { "None", "FBm", "Ridged", "PingPong" };
	if (ImGui::Combo("Fractal Type", &fractalType, fractalTypes, 4)) {
		regenerateTerrain = true;
	}
	
	if (ImGui::SliderInt("Fractal Octaves", &fractalOctaves, 1, 8)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Fractal Gain", &fractalGain, 0.0f, 1.0f)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Weighted Strength", &fractalWeightedStrength, 0.0f, 1.0f)) {
		regenerateTerrain = true;
	}
	if (fractalType == 3) { // PingPong
		if (ImGui::SliderFloat("PingPong Strength", &fractalPingPongStrength, 0.0f, 4.0f)) {
			regenerateTerrain = true;
		}
	}
	
	// Cellular noise specific controls
	if (noiseType == 2) { // Cellular
		ImGui::Separator();
		ImGui::Text("Cellular Noise Settings");
		ImGui::Separator();
		
		const char* distanceFunctions[] = { "Euclidean", "EuclideanSq", "Manhattan", "Hybrid" };
		if (ImGui::Combo("Distance Function", &cellularDistanceFunction, distanceFunctions, 4)) {
			regenerateTerrain = true;
		}
		
		const char* returnTypes[] = { "CellValue", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div" };
		if (ImGui::Combo("Return Type", &cellularReturnType, returnTypes, 7)) {
			regenerateTerrain = true;
		}
		
		if (ImGui::SliderFloat("Cellular Jitter", &cellularJitter, 0.0f, 1.0f)) {
			regenerateTerrain = true;
		}
	}
	
	ImGui::Separator();
	ImGui::Text("Domain Warp");
	ImGui::Separator();
	
	if (ImGui::Checkbox("Enable Domain Warp", &useDomainWarp)) {
		regenerateTerrain = true;
	}
	
	if (useDomainWarp) {
		const char* domainWarpTypes[] = { "OpenSimplex2", "OpenSimplex2Reduced", "BasicGrid" };
		if (ImGui::Combo("Warp Type", &domainWarpType, domainWarpTypes, 3)) {
			regenerateTerrain = true;
		}
		
		if (ImGui::SliderFloat("Warp Amplitude", &domainWarpAmp, 0.0f, 100.0f)) {
			regenerateTerrain = true;
		}
		if (ImGui::SliderFloat("Warp Frequency", &domainWarpFreq, 0.01f, 0.2f)) {
			regenerateTerrain = true;
		}
	}
	
	ImGui::Separator();
	ImGui::Text("Noise Drift (Fog Effect)");
	ImGui::Separator();
	
	if (ImGui::Checkbox("Enable Noise Drift", &enableNoiseDrift)) {
		if (!enableNoiseDrift) {
			regenerateTerrain = true; // Regenerate once when disabling to reset
		}
	}
	
	if (enableNoiseDrift) {
		if (ImGui::SliderFloat("Drift Speed", &noiseDriftSpeed, 0.001f, 1.0f)) {
			// Speed change doesn't require regeneration
		}
		
		if (ImGui::Button("Reset Drift Time", ImVec2(-1, 0))) {
			noiseTime = 0.0f;
			regenerateTerrain = true;
		}
		
		ImGui::Text("Current Time: %.2f", noiseTime);
	}
	
	ImGui::Separator();
	ImGui::Text("Render Settings");
	ImGui::Separator();
	
	const char* renderModes[] = { "Wireframe", "Fill", "Points" };
	ImGui::Combo("Render Mode", &renderMode, renderModes, 3);
	
	if (ImGui::Button("Regenerate Terrain", ImVec2(-1, 0))) {
		regenerateTerrain = true;
	}
	
	ImGui::Separator();
	ImGui::Text("Light Parameters");
	ImGui::Separator();
	
	ImGui::SliderFloat("Light X", &lightPosX, -100.0f, 100.0f);
	ImGui::SliderFloat("Light Y", &lightPosY, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Z", &lightPosZ, -100.0f, 100.0f);
	
	ImGui::ColorEdit3("Light Color", &lightColorR);
	ImGui::SliderFloat("Light Scale", &lightScale, 0.1f, 5.0f);
	
	if (ImGui::Button("Reset Light", ImVec2(-1, 0))) {
		lightPosX = 0.0f;
		lightPosY = 50.0f;
		lightPosZ = 0.0f;
		lightColorR = 1.0f;
		lightColorG = 1.0f;
		lightColorB = 1.0f;
		lightScale = 2.0f;
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