#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTex;
layout(location=2) in uint offset;

out vec2 textCoords;

uniform mat4 mvp;

vec3 unpack(uint data, vec3 position) {
    float x = float(data       & 15u);
    float y = float(data >> 4  & 15u);
    float z = float(data >> 8  & 15u);
    uint dir =      data >> 12 & 7u;

    vec3 offset = vec3(x,y,z);
    vec3 pos = position;

    switch(dir) {
        case 1u:
            pos = vec3(-position.x,position.y,-position.z);
            break;
        case 2u:
            pos = vec3(position.z,position.y,-position.x);
            break;
        case 3u:
            pos = vec3(-position.z,position.y,position.x);
            break;
        case 4u:
            pos = vec3(position.x,-position.z,position.y);
            break;
        case 5u:
            pos = vec3(position.x,position.z,-position.y);
            break;
    }
    
    return pos+offset;
}

void main() {
    gl_Position = mvp * vec4(unpack(offset, aPos), 1.0);
    textCoords = aTex;
}