#version 410 core
precision mediump float;

uniform bool isBright;
uniform float intensity;

in vec4 vColor;

out vec4 fragColor;

void main() {
    fragColor = isBright ? vec4(vec3(1.0), intensity) : vec4(0.0);
}