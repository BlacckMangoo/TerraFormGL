#include <CameraWindow.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Camera.h>

void CameraWindow::RenderUi(const Camera& camera) {

    ImGui::SetNextWindowPos(ImVec2(1000, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);

    ImGui::Begin("Camera Settings");

    ImGui::Text("Camera Position");
    ImGui::Text("X: %.1f", camera.cameraPos.x);
    ImGui::Text("Y: %.1f", camera.cameraPos.y);
    ImGui::Text("Z: %.1f", camera.cameraPos.z);

    ImGui::Text("Camera Rotation");
    ImGui::Text("Yaw: %.1f", camera.yaw);
    ImGui::Text("Pitch: %.1f", camera.pitch);

    ImGui::End();
}

void CameraWindow::Close() {
   ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
