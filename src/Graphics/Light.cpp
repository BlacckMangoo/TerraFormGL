#include "../../include/Graphics/Light.h"
#include "../../include/Core/IClickable.h"
#include "Graphics/LightWindow.h"
#include <iostream>

Light::Light(const glm::vec3& pos, const glm::vec3& col, float rad)
    : position(pos), color(col), radius(rad), VAO(0), VBO(0), EBO(0), initialized(false)
{
	// Initialize the light cube geometry
	setupGeometry();
	initialized = true;
    
    // Create the UI window for this light
    lightWindow = new LightWindow(this);
}

Light::~Light()
{
    if (initialized) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    
    // Clean up UI window
    if (lightWindow) {
        delete lightWindow;
        lightWindow = nullptr;
    }
}


void Light::setupGeometry()
{
    // Cube vertices (position only)
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Left face
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Right face
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        // Top face
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Light::Render( Shader& lightShader, const Camera& camera)
{
    if (!initialized) return;

    lightShader.Use();

    // Render the main light cube at position
    {
        // Create transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(radius)); // Scale based on radius

        glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 16.0f/9.0f, 0.01f, 2000.0f);
        glm::mat4 mvp = projection * view * model;

        // Set uniforms
        GLuint mvpLoc = glGetUniformLocation(lightShader.ID, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        lightShader.SetVector3f("lightColor", color.x, color.y, color.z);
        lightShader.SetVector3f("lightPosition", position.x, position.y, position.z);

        // Render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    
    // Draw a wireframe sphere to show the clickable radius
    {
        // Enable wireframe mode
        GLint polygonMode;
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // Create a slightly larger wireframe cube to indicate clickable area
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(radius * 1.1f)); 
        
        glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 16.0f/9.0f, 0.01f, 2000.0f);
        glm::mat4 mvp = projection * view * model;
        
        // Set uniforms
        GLuint mvpLoc = glGetUniformLocation(lightShader.ID, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        
        // Use yellow for the clickable area visualization
        lightShader.SetVector3f("lightColor", 1.0f, 1.0f, 0.0f);
        lightShader.SetVector3f("lightPosition", position.x, position.y, position.z);
        
        // Render the wireframe cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // Restore original polygon mode
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }
}

void Light::SetPosition(const glm::vec3& pos)
{
    // Simply update position
    position = pos;
}

void Light::SetColor(const glm::vec3& col)
{
    color = col;
}

void Light::SetRadius(float newRadius)
{
    // Set a minimum radius to prevent issues
    radius = std::max(newRadius, 0.1f);
}

bool Light::IntersectsRay(const Ray& ray, float& tHit) const
{
    // Use position for intersection test
    glm::vec3 oc = ray.origin - position;
    float a = glm::dot(ray.direction, ray.direction); // Should be 1.0 if direction is normalized
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return false; // No intersection
    }
    
    float t0 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
    
    // Get the nearest positive intersection
    if (t0 > 0 && t1 > 0) {
        tHit = std::min(t0, t1);
        
        // Calculate the actual hit point
        glm::vec3 hitPoint = ray.origin + ray.direction * tHit;
    } else if (t0 > 0) {
        tHit = t0;
    } else if (t1 > 0) {
        tHit = t1;
    } else {
        return false; // Both intersections are behind the ray origin
    }
    
    return true;
}
