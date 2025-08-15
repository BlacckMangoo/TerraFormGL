#version 330 core 

in vec3 FragPos;
out vec4 FragColor;
uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}