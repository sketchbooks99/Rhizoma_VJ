#version 410 core

uniform mat4 mvpMatrix;

in vec3 normal;
in vec4 position;
in vec2 texcoord;
in vec4 color;

out vec3 vNormal;
out vec4 vColor;
out vec2 vTexCoord;

void main() {
    vNormal = normal;
    vTexCoord = texcoord;
    vColor = color;
    gl_Position = mvpMatrix * position;
}