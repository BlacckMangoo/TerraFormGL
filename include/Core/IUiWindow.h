#pragma once 

// Forward declaration to avoid circular includes
class Camera;

class IUiWindow
{
public:
    virtual void RenderUi(const Camera& camera) = 0;
   
};
