#pragma once
#include <Core/Camera.h>
#include <Graphics/Shader.h>
class IRenderable {
public:
    virtual void Init() = 0;
    virtual void Draw(const Camera& camera, Shader shader) = 0;
    virtual void Update(float dt) = 0;
};