#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTex;

out vec2 textCoords;

uniform mat4 mvp;
uniform uint[4096];

vec3 unpack(uint data, vec3 position) {
    float x = data >> 12 & 16u;
    float y = data >> 8  & 16u;
    float z = data >> 4  & 16u;
    int 
    
    return vec3(x,y,z);
}

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    textCoords = aTex;
}