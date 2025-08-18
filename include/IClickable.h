#pragma once

#include <glm/glm.hpp>


struct Ray {
    glm::vec3 origin;
    glm::vec3 direction; // normalized
};

class ISelectable {
public:
    virtual ~ISelectable() = default;
    virtual bool IntersectsRay(const Ray& ray, float& tHit) const = 0;
};
