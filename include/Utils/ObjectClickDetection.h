#pragma once
#include "Core/IClickable.h"
#include "../Core/Camera.h"
class ObjectClickDetection {
    public:

    Ray  generateRay(int x, int y , int screenWidth , int screenHeight , const Camera& camera );

};