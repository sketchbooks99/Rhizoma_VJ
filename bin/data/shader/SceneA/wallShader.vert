#version 440

#pragma include "../util/util.frag"

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

// out float volume;
out vec4 vColor;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
    vColor = vec4(1.0);
}