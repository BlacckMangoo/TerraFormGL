#include "UiManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"



void UiManager::RenderUi(Camera& camera)
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


void UiManager::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}