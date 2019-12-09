#version 410 core

uniform bool isBright;

in vec4 vColor;

out vec4 fragColor;

void main() {
    fragColor = isBright ? vec4(1.0) : vec4(0.0);
}