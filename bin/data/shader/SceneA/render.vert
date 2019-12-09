#version 410 core

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 color;

out vec4 vColor;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
    vColor = color;
}