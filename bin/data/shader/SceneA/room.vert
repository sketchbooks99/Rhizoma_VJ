#version 440

// uniform mat4 mvpMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 view;
uniform mat4 model;

in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

out vec3 vNormal;
out vec4 vColor;
out vec4 vPosition;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
    vNormal = normal;
    vColor = color;
    vPosition = view * position;
}