#version 330 core 

layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 u_MVP;


void main()
{
    FragPos = aPos;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}