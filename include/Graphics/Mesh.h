#pragma once
#include <Graphics/Shader.h>
#include "Core/IRenderable.h"
#include <glm/glm.hpp>
#include <vector>
#include <Core/Camera.h>

#include "Core/IClickable.h"

struct MeshData {
    std::vector<glm::vec3> vertices ;
    std::vector<unsigned int> indices ;

};

struct PhysicsProperties {
    float mass = 100.0f; 
    glm::vec3 velocity = {0, 0, 0};
    glm::vec3 acceleration = {0, 0, 0};
};

struct Transform {
    glm::vec3 position = {0,0,0};
    glm::vec3 rotation = {0,0,0};
    glm::vec3 scale = {1,1,1};

};

class Mesh : public IRenderable {
public:
    void Init() override;
    void Draw(const Camera& camera, Shader shader) override;
    void Update(float dt) override;

    Transform transform;
    PhysicsProperties physicsProperties;
    MeshData meshData;
    glm::vec4 color = {0.8, 0.2, 0.3, 1.0};

    Shader* shader;


    Mesh(Transform transform, MeshData meshData, Shader* shader = nullptr , PhysicsProperties physicsProperties = PhysicsProperties(), glm::vec4 color = {0.8, 0.2, 0.3, 1.0});
    ~Mesh();

    // Method to update vertex positions in the mesh
    void UpdateVertexPositions();

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

};
