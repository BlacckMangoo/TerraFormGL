#pragma once
#include <Camera.h>
#include "IUiWindow.h"

enum RenderModes
{
    RENDER_MODE_WIRE_FRAME,
    RENDER_MODE_FILL,
    RENDER_MODE_POINTS

};


class TerrainWindow : public IUiWindow {

public:
    // Terrain parameters for ImGui controls
    int terrainWidth = 50;
    int terrainHeight = 50;
    float terrainScale = 1.0f;
    float terrainFrequency = 0.1f;
    float terrainAmplitude = 0.0f;
    bool regenerateTerrain = true;
    int renderMode = 0; // 0=wireframe, 1=fill, 2=points

	RenderModes mode  = RENDER_MODE_WIRE_FRAME; // Start with wireframe mode




	TerrainWindow() = default;
	~TerrainWindow() = default;

	void RenderUi(const Camera& camera) override;
	void Close() override;

	RenderModes GetRenderMode() const {
		return mode;
	}

	void SetRenderMode(int newMode) {
		renderMode = newMode;
		// Also update the enum mode
		switch (newMode) {
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
};