#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"

class Light
{
public:
    glm::vec3 position;
    glm::vec3 color;
    float scale;

    // Constructor
    Light(const glm::vec3& pos = glm::vec3(0.0f, 10.0f, 0.0f), 
          const glm::vec3& col = glm::vec3(1.0f, 1.0f, 1.0f),
          float lightScale = 1.0f);
    
    // Destructor
    ~Light();

    // Initialize the light cube geometry
    void Init();
    
    // Render the light cube
    void Render( Shader& lightShader, const Camera& camera);
    
    // Update light properties
    void SetPosition(const glm::vec3& pos);
    void SetColor(const glm::vec3& col);
    void SetScale(float lightScale);

private:
    unsigned int VAO, VBO,EBO;
    bool initialized;
    
    void setupGeometry();
};
