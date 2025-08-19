#pragma once 
#include "IUiWindow.h"
#include <glm/glm.hpp>

// Forward declaration
class Light;
class Camera;

class LightWindow : public IUiWindow
{
public:
    LightWindow(Light* light = nullptr);
    ~LightWindow();

    // Implement IUiWindow interface
    virtual void RenderUi(const Camera& camera) override;
    virtual void Close() override;

    // Toggle window visibility
    void Toggle();
    
    // Set the light this window controls
    void SetLight(Light* light);
    
    // Get window state
    bool IsOpen() const { return isOpen; }

private:
    Light* light;  // The light this window controls
    bool isOpen;   // Is the window currently displayed
    
    // Copy of light properties to detect changes
    glm::vec3 position;
    glm::vec3 color;
};
