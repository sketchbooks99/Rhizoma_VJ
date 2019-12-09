#version 410 core

uniform mat4 modelViewProjectionMatrix;

in vec4 color;
in vec2 texcoord;
in vec4 position;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
    // gl_Position = position;
    vTexCoord = texcoord;
    vColor = color;
}