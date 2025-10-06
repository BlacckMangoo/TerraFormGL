#version 430
layout(local_size_x = 128) in; // must match LOCAL_SIZE_X in C++

// use a struct for clarity (std430 will pack vec3 to 16 bytes each unless arranged)
layout(std430, binding = 0) buffer TerrainData {
    float vertices[]; // plain float array: position.xyz normal.xyz position.xyz normal.xyz ...
};

uniform float deltaTime;

void main() {
    uint gid = gl_GlobalInvocationID.x;
    // Each vertex has 6 floats: pos.x,y,z, normal.x,y,z
    uint base = gid * 6u;




    vec3 pos = vec3(vertices[base + 0], vertices[base + 1], vertices[base + 2]);
    float heightOffset = sin(pos.x * 0.4 + deltaTime) * 0.001;
    pos.y += heightOffset;
    vertices[base + 1] = pos.y;


}
