#pragma once 

// Forward declaration to avoid circular includes
class Camera;

class IUiManager
{
public:
    virtual void RenderUi(const Camera& camera) = 0;
    virtual void Close() = 0;
   
};
