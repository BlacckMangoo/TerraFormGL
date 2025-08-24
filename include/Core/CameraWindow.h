#pragma once 
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Core/IUiWindow.h>

// Forward declaration
class Camera;

class CameraWindow : public IUiWindow {
public:
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  float fov = 45.0f;
	void RenderUi(const Camera& camera) override;

};