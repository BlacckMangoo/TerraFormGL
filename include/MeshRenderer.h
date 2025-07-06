#pragma once 
#include "Shader.h"
#include "Camera.h"

enum RenderModes
{
	RENDER_MODE_WIRE_FRAME,
	RENDER_MODE_FILL,
	RENDER_MODE_POINTS

};


class MeshRenderer {
public:

    // Constructor (inits shaders/shapes)
    MeshRenderer(Shader& shader);
    MeshRenderer() = default;
    ~MeshRenderer();
    // Renders a defined quad textured with given sprite

    int vertexCount; 

    void Draw(RenderModes mode, const Camera& camera );
private:
    // Render state
    Shader       shader;
    unsigned int    VAO;
    unsigned int VBO;


    void initRenderData();
};