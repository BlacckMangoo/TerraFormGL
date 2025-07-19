#version 330 core
out vec4 FragColor;

in vec3 Normal; 
in vec3 FragPos;

uniform vec3 objectColor;
uniform int numLights;
uniform vec3 lightPositions[3];
uniform vec3 lightColors[3];

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);
    float ambientStrength = 0.1;
    for (int i = 0; i < numLights; ++i) {
        // Ambient
        vec3 ambient = ambientStrength * lightColors[i];
        // Diffuse
        vec3 lightDir = normalize(lightPositions[i] - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColors[i];
        result += (ambient + diffuse);
    }
    result *= objectColor;
    FragColor = vec4(result, 1.0);
}