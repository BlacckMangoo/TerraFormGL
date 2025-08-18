#pragma once
#include "IClickable.h"
#include "glm/glm.hpp"
#include "Camera.h"
class ObjectClickDetection {
    public:

    Ray  generateRay(int x, int y , int screenWidth , int screenHeight , const Camera& camera );

};