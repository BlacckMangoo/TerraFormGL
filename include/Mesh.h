#pragma once
#include <Shader.h>
#include "IRenderable.h"
#include <glm/glm.hpp>
#include <vector>
#include <Camera.h>


// make sure the shader has a uModel uniform
//default Mesh Data is for a cube 




struct MeshData {
    std::vector<glm::vec3> vertices ;
    std::vector<unsigned int> indices ;

};

struct PhysicsProperties {
    float mass = 1.0f;
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

    Shader* shader;


    Mesh(Transform transform, MeshData meshData, Shader* shader = nullptr , PhysicsProperties physicsProperties = PhysicsProperties());
    ~Mesh();

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

};
