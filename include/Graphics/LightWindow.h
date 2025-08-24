#pragma once 
#include "../Core/IUiWindow.h"
#include <glm/glm.hpp>

// Forward declaration
class Light;
class Camera;

class LightWindow : public IUiWindow
{
public:
    LightWindow(Light* light = nullptr);

    // Implement IUiWindow interface
    virtual void RenderUi(const Camera& camera) override;
    void Toggle();
    void SetLight(Light* light);
    bool IsOpen() const { return isOpen; }
private:
    Light* light;
    bool isOpen;
    glm::vec3 position;
    glm::vec3 color;
};
