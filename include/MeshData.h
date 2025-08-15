#pragma once 

#include <glm/glm.hpp>
#include <vector>
#include <Mesh.h>


MeshData cubeData = {
   // vertices
    {
        // Front face
        {-0.5f, -0.5f,  0.5f}, 
        { 0.5f, -0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 

        // Back face
        {-0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f, -0.5f}, 
        { 0.5f,  0.5f, -0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 

        // Left face
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 

        // Right face
        {0.5f, -0.5f, -0.5f}, 
        {0.5f, -0.5f,  0.5f}, 
        {0.5f,  0.5f,  0.5f}, 
        {0.5f,  0.5f, -0.5f}, 

        // Top face
        {-0.5f,  0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        { 0.5f,  0.5f, -0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 

        // Bottom face
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
    },

    // indices
    {
        // Front
        0, 1, 2,  2, 3, 0,
        // Back
        4, 6, 5,  6, 4, 7,
        // Left
        8, 9,10, 10,11, 8,
        // Right
       12,14,13, 14,12,15,
       // Top
      16,17,18, 18,19,16,
      // Bottom
     20,22,21, 22,20,23
  }
};


MeshData planeData = {
    // vertices
    {
        {-0.5f, 0.0f, -0.5f},
        { 0.5f, 0.0f, -0.5f},
        { 0.5f, 0.0f,  0.5f},
        {-0.5f, 0.0f,  0.5f},
    },

    // indices
    {
        0, 1, 2,  2, 3, 0
    }
};

MeshData triangleData = {
    // vertices
    {
        { 0.0f,  0.5f,  0.0f},
        {-0.5f, -0.5f,  0.0f},
        { 0.5f, -0.5f,  0.0f},
    },

    // indices
    {
        0, 1, 2
    }
};


MeshData GenerateCircleMeshData (float radius, unsigned int segments , glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f))
{
    MeshData circleData;
    
    // Add center vertex
    circleData.vertices.push_back(center);
    
    // Generate perimeter vertices
    for (unsigned int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / static_cast<float>(segments);
        circleData.vertices.push_back({
            radius * cos(angle) + center.x,
         
            radius * sin(angle) + center.z,
            0.0f + center.y
        });
    }

    // Generate indices - fan triangulation from center (counter-clockwise winding)
    for (unsigned int i = 0; i < segments; ++i) {
        unsigned int next = (i + 1) % segments + 1;
        circleData.indices.push_back(0); // Center vertex
        circleData.indices.push_back(i + 1); // Current perimeter vertex
        circleData.indices.push_back(next); // Next perimeter vertex
    }

    return circleData;
}


MeshData GenerateSphereMeshData(float radius, unsigned int segments, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) {
    MeshData sphereData;
    const float PI = 3.14159265358979323846f;

    for (unsigned int i = 0; i <= segments; ++i) {
        float theta = i * PI / segments; // latitude angle
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int j = 0; j <= segments; ++j) {
            float phi = j * 2 * PI / segments; // longitude angle
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 vertex;
            vertex.x = center.x + radius * cosPhi * sinTheta;
            vertex.y = center.y + radius * cosTheta;
            vertex.z = center.z + radius * sinPhi * sinTheta;

            sphereData.vertices.push_back(vertex);
        }
    }

    for (unsigned int i = 0; i < segments; ++i) {
        for (unsigned int j = 0; j < segments; ++j) {
            unsigned int first = (i * (segments + 1)) + j;
            unsigned int second = first + segments + 1;

            sphereData.indices.push_back(first);
            sphereData.indices.push_back(second);
            sphereData.indices.push_back(first + 1);

            sphereData.indices.push_back(second);
            sphereData.indices.push_back(second + 1);
            sphereData.indices.push_back(first + 1);
        }
    }

    return sphereData;
}