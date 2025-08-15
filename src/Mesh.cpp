#include <Mesh.h>
#include <iostream>

Mesh::Mesh(Transform transform, MeshData meshData, Shader* shader , PhysicsProperties physicsProperties)
    :  transform(transform), meshData(meshData), shader(shader), physicsProperties(physicsProperties) {
    Init();
}

Mesh::~Mesh() {
    // Cleanup resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::Init()   {
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(glm::vec3), meshData.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0); 
    glEnableVertexAttribArray(0);

    if (!meshData.indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), 
                     meshData.indices.data(), GL_STATIC_DRAW);
    } 
    
    glBindVertexArray(0);
}

void Mesh::Draw(const Camera& camera, Shader shader) {
    // Remove the GL_LINE mode to see the filled shape
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale);

        glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 16.0f/9.0f, 0.01f, 2000.0f);
        glm::mat4 mvp = projection * view * model;

        // Get uniform locations
        GLint mvpLoc = glGetUniformLocation(shader.ID, "u_MVP");

        if (mvpLoc == -1) {
            std::cout << "Failed to find u_MVP uniform in shader " << shader.ID << std::endl;
        } else {
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        }
        glBindVertexArray(VAO);
     
        if (!meshData.indices.empty()) {
            glDrawElements(GL_TRIANGLES, meshData.indices.size(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, meshData.vertices.size());
        }
        
        glBindVertexArray(0);
    }


void Mesh::Update(float dt) {

    // verlet integration 

     transform.position += physicsProperties.velocity * dt;
    physicsProperties.velocity += physicsProperties.acceleration * physicsProperties.mass * dt;
    

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale);


    if (shader) {
        shader->SetMatrix4("u_Model", model);
    }
   
}