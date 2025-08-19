#include "LightWindow.h"
#include "Light.h"
#include "Camera.h"
#include <imgui.h>
#include <iostream>

LightWindow::LightWindow(Light* light)
    : light(light), isOpen(false), position(0.0f), color(1.0f)
{
    if (light) {
        position = light->position;
        color = light->color;
    }
}

LightWindow::~LightWindow()
{
    // Nothing to clean up
}

void LightWindow::RenderUi(const Camera& camera)
{
    if (!isOpen || !light) return;

    // Create a window for the light controls
    if (ImGui::Begin("Light Controls", &isOpen))
    {
        // Capture initial values to detect changes
        glm::vec3 oldPosition = position;
        glm::vec3 oldColor = color;
        
        // Show position sliders
        ImGui::Text("Position");
        ImGui::SliderFloat("X##pos", &position.x, -50.0f, 50.0f);
        ImGui::SliderFloat("Y##pos", &position.y, 0.0f, 50.0f);
        ImGui::SliderFloat("Z##pos", &position.z, -50.0f, 50.0f);
        
        ImGui::Separator();
        
        // Show color sliders
        ImGui::Text("Color");
        ImGui::ColorEdit3("RGB", &color.x);
        
        // Size control
        ImGui::Separator();
        ImGui::Text("Light Size");
        float radius = light->radius;
        if (ImGui::SliderFloat("Radius", &radius, 0.1f, 5.0f)) {
            light->SetRadius(radius);
        }
        
        // If values changed, update the light
        if (oldPosition != position) {
            light->SetPosition(position);
        }
        
        if (oldColor != color) {
            light->SetColor(color);
        }
    }
    ImGui::End();
}

void LightWindow::Close()
{
    isOpen = false;
}

void LightWindow::Toggle()
{
    isOpen = !isOpen;
    
    if (isOpen && light) {
        // Update local copy of values when window opens
        position = light->position;
        color = light->color;
    }
}

void LightWindow::SetLight(Light* lightPtr)
{
    light = lightPtr;
    if (light) {
        position = light->position;
        color = light->color;
    }
}
