#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui.h"
Camera::Camera()
{
    cameraPos = glm::vec3(25.0f, 15.0f, 25.0f);  // Start at a good position to see terrain
    cameraFront = glm::vec3(-0.5f, -0.3f, -0.5f);  // Look down and toward terrain center
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    updateViewMatrix(); // Initialize view matrix
}

void Camera::processInput(GLFWwindow* window, float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::updateViewMatrix()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processMouseMovement(float xpos, float ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    // Handle camera rotation (right mouse button)
    if (isRotating)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Constrain pitch to prevent screen flipping
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Update camera front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
    // Handle camera panning (middle mouse button)
    if (isPanning)
    {
        xoffset *= panSensitivity;
        yoffset *= panSensitivity;

        // Calculate right and up vectors for the camera
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));

        // Move camera position based on mouse movement
        cameraPos -= right * xoffset;
        cameraPos += up * yoffset;
    }
}

void Camera::processMouseScroll(float yoffset)
{
    fov -= yoffset * zoomSensitivity;
    
    // Clamp FOV to reasonable limits
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 120.0f)
        fov = 120.0f;
}

void Camera::processMouseButton(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            isRotating = true;
        else if (action == GLFW_RELEASE)
            isRotating = false;
    }
    
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
            isPanning = true;
        else if (action == GLFW_RELEASE)
            isPanning = false;
    }
}
