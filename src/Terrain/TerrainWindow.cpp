#include "Terrain/TerrainWindow.h"
#include "imgui.h"
#include "Core/Camera.h"



void TerrainWindow::RenderUi(const Camera& camera)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(380, 700), ImGuiCond_FirstUseEver);

	ImGui::Begin("Terrain Controls");

	ImGui::Text("Basic Terrain Parameters");
	ImGui::Separator();

	if (ImGui::SliderInt("Width", &terrainWidth, 1, 3000)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderInt("Height", &terrainHeight, 1, 3000)) {
		regenerateTerrain = true;
	}
	if (ImGui::SliderFloat("Scale", &terrainScale, 0.1f, 5.0f)) {
		regenerateTerrain = true;
	}


	ImGui::Separator();
	ImGui::Text("Render Settings");
	ImGui::Separator();
	// Make sure renderMode int matches the mode enum
	if (mode == RENDER_MODE_WIRE_FRAME && renderMode != 0)
		renderMode = 0;
	else if (mode == RENDER_MODE_FILL && renderMode != 1)
		renderMode = 1;
	else if (mode == RENDER_MODE_POINTS && renderMode != 2)
		renderMode = 2;

	const char* renderModes[] = { "Wireframe", "Fill", "Points" };
	if (ImGui::Combo("Render Mode", &renderMode, renderModes, 3)) {
		// Update the actual render mode enum when selection changes
		switch (renderMode) {
		case 0:
			mode = RENDER_MODE_WIRE_FRAME;
			break;
		case 1:
			mode = RENDER_MODE_FILL;
			break;
		case 2:
			mode = RENDER_MODE_POINTS;
			break;
		}
	}
	if (ImGui::Button("Regenerate Terrain", ImVec2(-1, 0))) {
		regenerateTerrain = true;
	}

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
	
}

