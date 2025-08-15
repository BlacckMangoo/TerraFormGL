#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos; 

uniform mat4 u_Model;
uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(aPos, 1.0);
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    Normal = mat3(transpose((u_Model))) * aNormal;
}