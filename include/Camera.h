#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW/glfw3.h"




class Camera {
public:
    Camera();

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 view;


    float yaw = -90.0f;     // Facing -Z
    float pitch = 0.0f;

    // Mouse tracking
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
    float mouseSensitivity = 0.1f;

    void processInput(GLFWwindow* window, float deltaTime);
    void updateViewMatrix();
    void processMouseMovement(float xpos, float ypos);


	
};