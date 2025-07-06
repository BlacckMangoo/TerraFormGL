#include "App.h"
#include "MeshRenderer.h"
#include "ResourceManger.h"
#include "Camera.h"

MeshRenderer* meshRenderer;
Camera camera; 


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}


App::App(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

App::~App()
{
  

}



void App::Init()
{

	
	ResourceManager::LoadShader(RESOURCES_PATH "shaders/mesh.vert", RESOURCES_PATH "shaders/mesh.frag", nullptr, "mesh");
	
	Shader meshShader = ResourceManager::GetShader("mesh");
	glfwSetCursorPosCallback(glfwGetCurrentContext(), mouseCallback);
	
	meshRenderer = new MeshRenderer(meshShader);
	
	glEnable(GL_DEPTH_TEST);

}


void App::Update(float dt)
{


}



void App::ProcessInput(float dt)
{// Process input for camera movement
	
	camera.processInput(glfwGetCurrentContext(), dt);


}

void App::Render()
{
	camera.updateViewMatrix();
	meshRenderer->Draw(RENDER_MODE_WIRE_FRAME, camera);
	
  
}