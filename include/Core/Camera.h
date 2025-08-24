#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

// Forward declaration
class CameraWindow;

class Camera
{
public:
    Camera();
    ~Camera();

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 view;

 

    CameraWindow *cameraWindow;

    float yaw = -90.0f; // Facing -Z (this matches our cameraFront = (0,0,-1))
    float pitch = 0.0f; // Looking straight ahead
    float fov = 45.0f;  // Field of view for zooming

    // Method to update camera from UI window settings
    void updateFromWindow();

    // Method to recalculate direction vectors from yaw and pitch
    void updateDirectionVectors();

    // Mouse tracking
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
    float mouseSensitivity = 0.6f;
    float zoomSensitivity = 2.0f;
    float panSensitivity = 0.1f;

    // Mouse button states
    bool isRotating = false;
    bool isPanning = false;

    void processInput(GLFWwindow *window, float deltaTime);
    void updateViewMatrix();
    void processMouseMovement(float xpos, float ypos);
    void processMouseScroll(float yoffset);
    void processMouseButton(int button, int action);
    float getFOV() const { return fov; }
       glm::mat4 getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(getFOV()), 16.0f / 9.0f, 0.01f, 2000.0f);
    }
    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
};