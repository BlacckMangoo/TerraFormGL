#pragma once

#include <glm/glm.hpp>


struct Ray {
    glm::vec3 origin;
    glm::vec3 direction; // normalized
};

class IClickable     {
public:
    virtual ~IClickable() = default;
    virtual bool IntersectsRay(const Ray& ray, float& tHit) const = 0;
};
