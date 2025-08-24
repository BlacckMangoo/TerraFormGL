

#include "Utils/ObjectClickDetection.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>

Ray ObjectClickDetection::generateRay(int x, int y, int screenWidth, int screenHeight, const Camera& camera) {
    float ndcX = (2.0f * x / screenWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * y / screenHeight); // Flip Y to match OpenGL convention (bottom-left is -1,-1)

    glm::vec4 clipnear = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 clipfar = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    glm::mat4 invProj = glm::inverse(camera.getProjectionMatrix());

    glm::vec4 eyeNear = invProj * clipnear;
    glm::vec4 eyeFar  = invProj * clipfar;

    eyeNear /= eyeNear.w;
    eyeFar  /= eyeFar.w;

    glm::mat4 invView = glm::inverse(camera.view);

    glm::vec4 worldNear = invView * eyeNear;
    glm::vec4 worldFar  = invView * eyeFar;


    Ray ray;
    ray.origin    = glm::vec3(worldNear);
    ray.direction = glm::normalize(glm::vec3(worldFar - worldNear));

    return ray;


}
